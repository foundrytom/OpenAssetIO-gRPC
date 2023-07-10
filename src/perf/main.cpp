// SPDX-License-Identifier: Apache-2.0
// Copyright 2013-2023 The Foundry Visionmongers Ltd
#include <chrono>
#include <functional>
#include <iostream>
#include <memory>
#include <numeric>
#include <random>
#include <string>
#include <unordered_map>

#include <Python.h>

#include <openassetio/Context.hpp>
#include <openassetio/EntityReference.hpp>
#include <openassetio/TraitsData.hpp>
#include <openassetio/hostApi/HostInterface.hpp>
#include <openassetio/hostApi/Manager.hpp>
#include <openassetio/hostApi/ManagerFactory.hpp>
#include <openassetio/log/ConsoleLogger.hpp>
#include <openassetio/log/SeverityFilter.hpp>
#include <openassetio/python/hostApi.hpp>
#include <openassetio/trait/property.hpp>

#include <openassetio-grpc/GRPCManagerImplementationFactory.hpp>

using openassetio::Context;
using openassetio::ContextPtr;
using openassetio::EntityReference;
using openassetio::hostApi::ManagerFactory;
using openassetio::hostApi::ManagerFactoryPtr;
using openassetio::hostApi::ManagerPtr;
using openassetio::log::ConsoleLogger;
using openassetio::log::SeverityFilter;

namespace {
/// Number of raw test results to print before showing summary stats.
constexpr const std::size_t kResultsToShow = 5;

class TestHostInterface : public openassetio::hostApi::HostInterface {
  [[nodiscard]] openassetio::Identifier identifier() const override {
    return "org.openassetio.gRPC.perfTest";
  }

  [[nodiscard]] openassetio::Str displayName() const override {
    return "OpenAssetIO gRPC Performance Test Host";
  }
};

}  // namespace

///////////////////////////////////////////////////////////

struct Fixture {
  const size_t callCount = 1000;
  const std::string aBalRef = "bal:///anAsset";
  const openassetio::trait::TraitSet traitSet = {
      "openassetio-mediacreation:content.LocateableContent", "somethingElse"};
  const openassetio::ContextPtr context = openassetio::Context::make();
  ManagerPtr gprcManager = nullptr;
  ManagerPtr directManager = nullptr;
};

///////////////////////////////////////////////////////////
// Test cases:

void grpcResolveSingle(Fixture& fixture) {
  for (size_t i = 0; i < fixture.callCount; ++i) {
    const EntityReference ref = fixture.gprcManager->createEntityReference(fixture.aBalRef);

    fixture.gprcManager->resolve(
        {ref}, fixture.traitSet, fixture.context,
        // NOLINTNEXTLINE(performance-unnecessary-value-param)
        [](std::size_t idx, const openassetio::TraitsDataPtr& data) {
          (void)idx;
          (void)data;
        },
        // NOLINTNEXTLINE(performance-unnecessary-value-param)
        [](std::size_t idx, const openassetio::BatchElementError& error) {
          (void)idx;
          (void)error;
        });
  }
}

void directResolveSingle(Fixture& fixture) {
  for (size_t i = 0; i < fixture.callCount; ++i) {
    const EntityReference ref = fixture.directManager->createEntityReference(fixture.aBalRef);

    fixture.directManager->resolve(
        {ref}, fixture.traitSet, fixture.context,
        // NOLINTNEXTLINE(performance-unnecessary-value-param)
        [](std::size_t idx, const openassetio::TraitsDataPtr& data) {
          (void)idx;
          (void)data;
        },
        // NOLINTNEXTLINE(performance-unnecessary-value-param)
        [](std::size_t idx, const openassetio::BatchElementError& error) {
          (void)idx;
          (void)error;
        });
  }
}

void grpcResolveBatch(Fixture& fixture) {
  std::vector<EntityReference> refs;
  refs.reserve(fixture.callCount);
  for (size_t i = 0; i < fixture.callCount; ++i) {
    refs.push_back(fixture.gprcManager->createEntityReference(fixture.aBalRef));
  }

  fixture.gprcManager->resolve(
      refs, fixture.traitSet, fixture.context,
      // NOLINTNEXTLINE(performance-unnecessary-value-param)
      [](std::size_t idx, const openassetio::TraitsDataPtr& data) {
        (void)idx;
        (void)data;
      },
      // NOLINTNEXTLINE(performance-unnecessary-value-param)
      [](std::size_t idx, const openassetio::BatchElementError& error) {
        (void)idx;
        (void)error;
      });
}

void directResolveBatch(Fixture& fixture) {
  std::vector<EntityReference> refs;
  refs.reserve(fixture.callCount);
  for (size_t i = 0; i < fixture.callCount; ++i) {
    refs.push_back(fixture.gprcManager->createEntityReference(fixture.aBalRef));
  }

  fixture.directManager->resolve(
      refs, fixture.traitSet, fixture.context,
      // NOLINTNEXTLINE(performance-unnecessary-value-param)
      [](std::size_t idx, const openassetio::TraitsDataPtr& data) {
        (void)idx;
        (void)data;
      },
      // NOLINTNEXTLINE(performance-unnecessary-value-param)
      [](std::size_t idx, const openassetio::BatchElementError& error) {
        (void)idx;
        (void)error;
      });
}

///////////////////////////////////////////////////////////

/**
 * Utility to time the above test cases.
 */
struct Benchmarker {
 private:
  using Idx = std::size_t;
  using Epoch = std::size_t;
  using Size = std::size_t;
  using Fraction = double;
  using Duration = std::chrono::nanoseconds;
  static constexpr std::string_view kDurationSuffix = "ns";
  using Ratio = double;
  using Ratios = std::vector<Ratio>;
  using RatioPair = std::pair<Idx, Idx>;

  /**
   * Convenience to hold a test case function and its printable name.
   */
  struct Case {
    using CaseFn = void (*)(Fixture&);
    const CaseFn caseFn;
    const std::string name;
  };

  static constexpr Idx kGRPResolveCaseIdx = 0;
  static constexpr Idx kDirectResolveCaseIdx = 1;
  static constexpr Idx kGRPResolveBatchCaseIdx = 2;
  static constexpr Idx kDirectResolveBatchCaseIdx = 3;

  /// Cases to test.
  static inline const std::array kCases{Case{&grpcResolveSingle, "grpcResolve"},
                                        Case{&directResolveSingle, "directResolve"},
                                        Case{&grpcResolveBatch, "grpcResolveBatched"},
                                        Case{&directResolveBatch, "directResolveBatch"}};

  /// Pairs of cases to compare as ratio of their timings.
  static constexpr std::array kRatioPairs{
      RatioPair{kGRPResolveCaseIdx, kDirectResolveCaseIdx},
      RatioPair{kGRPResolveBatchCaseIdx, kDirectResolveBatchCaseIdx}};

  using CasesTiming = std::array<Duration, kCases.size()>;

 public:
  using CasesTimings = std::vector<CasesTiming>;

  Epoch numEpochs;
  Epoch numWarmupEpochs;

  /**
   * Perform benchmark after first warming up.
   *
   * We tend to see a slowdown in the first few epochs caused by
   * caching, CPU throttling, etc.
   */
  CasesTimings benchmark() {
    // Warm up.
    for (Epoch epoch = 0; epoch < numWarmupEpochs; ++epoch) {
      executeAndMeasureAllCases();
    }

    // Do benchmark.

    CasesTimings casesTimings(numEpochs);

    for (Epoch epoch = 0; epoch < numEpochs; ++epoch) {
      casesTimings[epoch] = executeAndMeasureAllCases();
    }
    return casesTimings;
  }

  /**
   * Dump test case timings and timing ratios between them.
   */
  static void dumpTimings(const CasesTimings& timings) {
    // Calculate timing ratios.

    std::array<Ratios, kRatioPairs.size()> ratios;
    for (Idx ratioIdx = 0; ratioIdx < kRatioPairs.size(); ++ratioIdx) {
      ratios[ratioIdx] =
          calcRatios(timings, kRatioPairs[ratioIdx].first, kRatioPairs[ratioIdx].second);
    }

    // Gather spreadsheet of data.

    using Row = std::vector<double>;
    using Matrix = std::vector<Row>;
    Matrix data(timings.size());

    for (Epoch epoch = 0; epoch < data.size(); ++epoch) {
      data[epoch].resize(kCases.size() + kRatioPairs.size());

      for (Idx caseIdx = 0; caseIdx < kCases.size(); ++caseIdx) {
        data[epoch][caseIdx] = static_cast<double>(timings[epoch][caseIdx].count());
      }

      for (Idx ratioIdx = 0; ratioIdx < kRatioPairs.size(); ++ratioIdx) {
        data[epoch][kCases.size() + ratioIdx] = ratios[ratioIdx][epoch];
      }
    }

    // Column headings.

    std::cout << kCases[0].name << " (" << kDurationSuffix << ")";
    for (Idx caseIdx = 1; caseIdx < kCases.size(); ++caseIdx) {
      std::cout << ", " << kCases[caseIdx].name << " (" << kDurationSuffix << ")";
    }
    for (const auto& [numeratorCaseIdx, deominatorCaseIdx] : kRatioPairs) {
      std::cout << ", " << kCases[numeratorCaseIdx].name << "/" << kCases[deominatorCaseIdx].name;
    }
    std::cout << "\n";

    // Print spreadsheet of data.
    Idx rowIdx;
    for (rowIdx = 0; rowIdx < data.size() && rowIdx < kResultsToShow; ++rowIdx) {
      std::cout << data[rowIdx][0];
      for (std::size_t columnIdx = 1; columnIdx < data[rowIdx].size(); ++columnIdx) {
        std::cout << ", " << data[rowIdx][columnIdx];
      }
      std::cout << "\n";
    }
    if (rowIdx < data.size()) {
      std::cout << std::flush;
      std::cerr << "... skipping rest ..." << std::endl;
    }

    // Summary statistics.

    std::cout << "\n";
    std::cout << "numerator/denominator, mean, std dev\n";

    for (Idx ratioIdx = 0; ratioIdx < kRatioPairs.size(); ++ratioIdx) {
      std::cout << kCases[kRatioPairs[ratioIdx].first].name << "/"
                << kCases[kRatioPairs[ratioIdx].second].name << ", ";
      const auto& [meanRatio, stdDevRatio] = ratioStats(ratios[ratioIdx]);
      std::cout << meanRatio << ", " << stdDevRatio << "\n";
    }
  }

 private:
  /**
   * Calculate the ratios of timings for each run between two test
   * cases.
   */
  static Ratios calcRatios(const CasesTimings& timings, Idx numeratorCaseIdx,
                           Idx denominatorCaseIdx) {
    std::vector<Ratio> ratios;
    ratios.reserve(timings.size());

    for (const auto& timing : timings) {
      ratios.push_back(calcRatio(timing, numeratorCaseIdx, denominatorCaseIdx));
    }
    return ratios;
  }

  /**
   * Calculate the ratio of timings for a single run between two test
   * cases.
   */
  static constexpr Ratio calcRatio(const CasesTiming& timing, Idx numeratorCaseIdx,
                                   Idx denominatorCaseIdx) {
    return static_cast<double>(timing[numeratorCaseIdx].count()) /
           static_cast<double>(timing[denominatorCaseIdx].count());
  }

  /**
   * Calculate mean and standard deviation of given timing ratio list.
   */
  static std::pair<Ratio, Ratio> ratioStats(const Ratios& ratios) {
    const Ratio meanRatio =
        std::reduce(ratios.begin(), ratios.end(), 0.0) / static_cast<Ratio>(ratios.size());

    const Ratio stdDevRatio =
        std::sqrt(std::transform_reduce(ratios.begin(), ratios.end(), 0.0, std::plus<Ratio>{},
                                        [meanRatio](const Ratio ratio) {
                                          return (ratio - meanRatio) * (ratio - meanRatio);
                                        }) /
                  static_cast<Ratio>(ratios.size()));

    return {meanRatio, stdDevRatio};
  }

  /**
   * Perform a single run of all test cases and measure the time it
   * takes to perform.
   */
  CasesTiming executeAndMeasureAllCases() {
    CasesTiming timing;
    for (Idx caseIdx = 0; caseIdx < kCases.size(); ++caseIdx) {
      timing[caseIdx] = executeAndMeasureCase(kCases[caseIdx].caseFn);
    }
    return timing;
  }

  /**
   * Measure the time a test case function takes to execute.
   */
  Duration executeAndMeasureCase(const Case::CaseFn& func) {
    openassetio::log::LoggerInterfacePtr logger = SeverityFilter::make(ConsoleLogger::make());
    openassetio::hostApi::HostInterfacePtr hostInterface = std::make_shared<TestHostInterface>();

    Fixture fixture;

    auto grpcImplFactory =
        openassetio::grpc::GRPCManagerImplementationFactory::make("0.0.0.0:51234", logger);

    fixture.gprcManager =
        ManagerFactory::defaultManagerForInterface(hostInterface, grpcImplFactory, logger);

    auto directImplFactory =
        openassetio::python::hostApi::createPythonPluginSystemManagerImplementationFactory(logger);

    fixture.directManager =
        ManagerFactory::defaultManagerForInterface(hostInterface, directImplFactory, logger);

    const auto start = std::chrono::high_resolution_clock::now();
    func(fixture);
    const auto stop = std::chrono::high_resolution_clock::now();

    return std::chrono::duration_cast<Duration>(stop - start);
  }
};

///////////////////////////////////////////////////////////

int main(int argc, const char** argv) {  // NOLINT(bugprone-exception-escape)

  constexpr int kExpectedArgs = 3;

  if (argc < kExpectedArgs) {
    throw std::runtime_error{
        "Insufficient arguments: <num epochs> <num warmups>"};
  }

  const auto numEpochs = static_cast<std::size_t>(std::atoi(argv[1]));
  const auto numWarmUpEpochs = static_cast<std::size_t>(std::atoi(argv[2]));

  std::cerr << "                      Epochs: " << numEpochs << std::endl;
  std::cerr << "              Warm up epochs: " << numWarmUpEpochs << std::endl;

  Py_Initialize();
  Py_BEGIN_ALLOW_THREADS
  {
    Benchmarker benchmarker{numEpochs, numWarmUpEpochs};

    std::cerr << "Begin benchmark..." << std::endl;

    // Perform benchmarking loop.

    const Benchmarker::CasesTimings timings = benchmarker.benchmark();

    // Dump the output.

    Benchmarker::dumpTimings(timings);
  }
  Py_END_ALLOW_THREADS
  Py_FinalizeEx();
}
