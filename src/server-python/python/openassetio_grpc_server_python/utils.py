import openassetio

from . import openassetio_pb2


class ProxyHostInterface(openassetio.hostApi.HostInterface):
    def __init__(self, identifier, display_name):
        self.__identifier = identifier
        self.__display_name = display_name
        super().__init__()

    def identifier(self):
        return self.__identifier

    def displayName(self):
        return self.__display_name


def info_dict_to_message(info_dict):
    bools = {}
    ints = {}
    floats = {}
    strings = {}

    for k, v in info_dict.items():
        if isinstance(v, str):
            strings[k] = v
        elif isinstance(v, float):
            floats[k] = v
        elif isinstance(v, bool):
            bools[k] = v
        elif isinstance(v, int):
            ints[k] = v

    return openassetio_pb2.InfoDictMsg(bools=bools, ints=ints, floats=floats, strings=strings)


def info_message_to_dict(message):
    info = {}
    for data in (message.bools, message.ints, message.floats, message.strings):
        for k, v in data.items():
            info[k] = v
    return info


def message_to_traits_data(message):
    traits_data = openassetio.TraitsData()
    for trait in message.trait:
        for data in (trait.bools, trait.ints, trait.floats, trait.strings):
            for k, v in data.items():
                traits_data.setTraitProperty(trait.id, k, v)
    return traits_data


def traits_data_to_message(traits_data):
    traits = []
    for trait_id in traits_data.traitSet():
        bools = {}
        ints = {}
        floats = {}
        strings = {}
        for k in traits_data.traitPropertyKeys(trait_id):
            v = traits_data.getTraitProperty(trait_id, k)
            if isinstance(v, str):
                strings[k] = v
            elif isinstance(v, float):
                floats[k] = v
            elif isinstance(v, bool):
                bools[k] = v
            elif isinstance(v, int):
                ints[k] = v
        traits.append(
            openassetio_pb2.TraitsDataMsg.TraitMsg(
                id=trait_id, bools=bools, ints=ints, floats=floats, strings=strings
            )
        )
    return openassetio_pb2.TraitsDataMsg(trait=traits)


def message_to_trait_set(message):
    return set(message.traitId)


def message_to_host_session(message, logger):
    host_interface = ProxyHostInterface(message.host.identifier, message.host.displayName)
    return openassetio.managerApi.HostSession(openassetio.managerApi.Host(host_interface), logger)


def message_to_context(message):
    ## TODO(tc): Add bridge for manager state object
    context = openassetio.Context(
        openassetio.Context.Access(message.access),
        openassetio.Context.Retention(message.retention),
        message_to_traits_data(message.locale),
        None,
    )
    context.locale.addTrait("openassetio-grpc:locale:GRPCProxy")
    return context


def batch_element_error_to_message(error):
    return openassetio_pb2.ErrorMsg(code=error.code, errorMessage=error.message)


def to_entity_refs(strings, manager, host_session):
    ## TODO(tc) We should really just bridge these into batch element errors
    refs = []
    for string in strings:
        if not manager.isEntityReferenceString(string, host_session):
            raise ValueError(
                f"{string} is not an entity refrence belonging to {manager.identifier()}"
            )
        refs.append(openassetio.EntityReference(string))
    return refs
