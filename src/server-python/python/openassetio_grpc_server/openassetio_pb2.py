# -*- coding: utf-8 -*-
# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: openassetio.proto
"""Generated protocol buffer code."""
from google.protobuf import descriptor as _descriptor
from google.protobuf import descriptor_pool as _descriptor_pool
from google.protobuf import symbol_database as _symbol_database
from google.protobuf.internal import builder as _builder
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()




DESCRIPTOR = _descriptor_pool.Default().AddSerializedFile(b'\n\x11openassetio.proto\x12\x16openassetio_grpc_proto\"\x0e\n\x0c\x45mptyRequest\"\x0f\n\rEmptyResponse\".\n\x08\x45rrorMsg\x12\x0c\n\x04\x63ode\x18\x01 \x01(\x05\x12\x14\n\x0c\x65rrorMessage\x18\x02 \x01(\t\"\xc7\x03\n\x0bInfoDictMsg\x12=\n\x05\x62ools\x18\x01 \x03(\x0b\x32..openassetio_grpc_proto.InfoDictMsg.BoolsEntry\x12;\n\x04ints\x18\x02 \x03(\x0b\x32-.openassetio_grpc_proto.InfoDictMsg.IntsEntry\x12?\n\x06\x66loats\x18\x03 \x03(\x0b\x32/.openassetio_grpc_proto.InfoDictMsg.FloatsEntry\x12\x41\n\x07strings\x18\x04 \x03(\x0b\x32\x30.openassetio_grpc_proto.InfoDictMsg.StringsEntry\x1a,\n\nBoolsEntry\x12\x0b\n\x03key\x18\x01 \x01(\t\x12\r\n\x05value\x18\x02 \x01(\x08:\x02\x38\x01\x1a+\n\tIntsEntry\x12\x0b\n\x03key\x18\x01 \x01(\t\x12\r\n\x05value\x18\x02 \x01(\x03:\x02\x38\x01\x1a-\n\x0b\x46loatsEntry\x12\x0b\n\x03key\x18\x01 \x01(\t\x12\r\n\x05value\x18\x02 \x01(\x01:\x02\x38\x01\x1a.\n\x0cStringsEntry\x12\x0b\n\x03key\x18\x01 \x01(\t\x12\r\n\x05value\x18\x02 \x01(\t:\x02\x38\x01\"2\n\x07HostMsg\x12\x12\n\nidentifier\x18\x01 \x01(\t\x12\x13\n\x0b\x64isplayName\x18\x02 \x01(\t\"?\n\x0eHostSessionMsg\x12-\n\x04host\x18\x01 \x01(\x0b\x32\x1f.openassetio_grpc_proto.HostMsg\"f\n\nContextMsg\x12\x0e\n\x06\x61\x63\x63\x65ss\x18\x01 \x01(\x05\x12\x11\n\tretention\x18\x02 \x01(\x05\x12\x35\n\x06locale\x18\x03 \x01(\x0b\x32%.openassetio_grpc_proto.TraitsDataMsg\"\x1e\n\x0bTraitSetMsg\x12\x0f\n\x07traitId\x18\x01 \x03(\t\"\xcd\x04\n\rTraitsDataMsg\x12=\n\x05trait\x18\x01 \x03(\x0b\x32..openassetio_grpc_proto.TraitsDataMsg.TraitMsg\x1a\xfc\x03\n\x08TraitMsg\x12\n\n\x02id\x18\x01 \x01(\t\x12H\n\x05\x62ools\x18\x02 \x03(\x0b\x32\x39.openassetio_grpc_proto.TraitsDataMsg.TraitMsg.BoolsEntry\x12\x46\n\x04ints\x18\x03 \x03(\x0b\x32\x38.openassetio_grpc_proto.TraitsDataMsg.TraitMsg.IntsEntry\x12J\n\x06\x66loats\x18\x04 \x03(\x0b\x32:.openassetio_grpc_proto.TraitsDataMsg.TraitMsg.FloatsEntry\x12L\n\x07strings\x18\x05 \x03(\x0b\x32;.openassetio_grpc_proto.TraitsDataMsg.TraitMsg.StringsEntry\x1a,\n\nBoolsEntry\x12\x0b\n\x03key\x18\x01 \x01(\t\x12\r\n\x05value\x18\x02 \x01(\x08:\x02\x38\x01\x1a+\n\tIntsEntry\x12\x0b\n\x03key\x18\x01 \x01(\t\x12\r\n\x05value\x18\x02 \x01(\x03:\x02\x38\x01\x1a-\n\x0b\x46loatsEntry\x12\x0b\n\x03key\x18\x01 \x01(\t\x12\r\n\x05value\x18\x02 \x01(\x01:\x02\x38\x01\x1a.\n\x0cStringsEntry\x12\x0b\n\x03key\x18\x01 \x01(\t\x12\r\n\x05value\x18\x02 \x01(\t:\x02\x38\x01\")\n\x13IdentifiersResponse\x12\x12\n\nidentifier\x18\x01 \x03(\t\"(\n\x12InstantiateRequest\x12\x12\n\nidentifier\x18\x01 \x01(\t\"%\n\x13InstantiateResponse\x12\x0e\n\x06handle\x18\x01 \x01(\t\" \n\x0e\x44\x65stroyRequest\x12\x0e\n\x06handle\x18\x01 \x01(\t\"#\n\x11IdentifierRequest\x12\x0e\n\x06handle\x18\x01 \x01(\t\"(\n\x12IdentifierResponse\x12\x12\n\nidentifier\x18\x01 \x01(\t\"$\n\x12\x44isplayNameRequest\x12\x0e\n\x06handle\x18\x01 \x01(\t\"*\n\x13\x44isplayNameResponse\x12\x13\n\x0b\x64isplayName\x18\x01 \x01(\t\"\x1d\n\x0bInfoRequest\x12\x0e\n\x06handle\x18\x01 \x01(\t\"A\n\x0cInfoResponse\x12\x31\n\x04info\x18\x01 \x01(\x0b\x32#.openassetio_grpc_proto.InfoDictMsg\"^\n\x0fSettingsRequest\x12\x0e\n\x06handle\x18\x01 \x01(\t\x12;\n\x0bhostSession\x18\x02 \x01(\x0b\x32&.openassetio_grpc_proto.HostSessionMsg\"I\n\x10SettingsResponse\x12\x35\n\x08settings\x18\x01 \x01(\x0b\x32#.openassetio_grpc_proto.InfoDictMsg\"\x97\x01\n\x11InitializeRequest\x12\x0e\n\x06handle\x18\x01 \x01(\t\x12;\n\x0bhostSession\x18\x02 \x01(\x0b\x32&.openassetio_grpc_proto.HostSessionMsg\x12\x35\n\x08settings\x18\x03 \x01(\x0b\x32#.openassetio_grpc_proto.InfoDictMsg\"\xd2\x01\n\x17ManagementPolicyRequest\x12\x0e\n\x06handle\x18\x01 \x01(\t\x12;\n\x0bhostSession\x18\x02 \x01(\x0b\x32&.openassetio_grpc_proto.HostSessionMsg\x12\x33\n\x07\x63ontext\x18\x03 \x01(\x0b\x32\".openassetio_grpc_proto.ContextMsg\x12\x35\n\x08traitSet\x18\x04 \x03(\x0b\x32#.openassetio_grpc_proto.TraitSetMsg\"Q\n\x18ManagementPolicyResponse\x12\x35\n\x06policy\x18\x01 \x03(\x0b\x32%.openassetio_grpc_proto.TraitsDataMsg\"\x81\x01\n\x1eIsEntityReferenceStringRequest\x12\x0e\n\x06handle\x18\x01 \x01(\t\x12;\n\x0bhostSession\x18\x02 \x01(\x0b\x32&.openassetio_grpc_proto.HostSessionMsg\x12\x12\n\nsomeString\x18\x03 \x01(\t\"-\n\x1fIsEntityReferenceStringResponse\x12\n\n\x02is\x18\x01 \x01(\x08\"\xe2\x01\n\x0eResolveRequest\x12\x0e\n\x06handle\x18\x01 \x01(\t\x12;\n\x0bhostSession\x18\x02 \x01(\x0b\x32&.openassetio_grpc_proto.HostSessionMsg\x12\x33\n\x07\x63ontext\x18\x03 \x01(\x0b\x32\".openassetio_grpc_proto.ContextMsg\x12\x35\n\x08traitSet\x18\x04 \x01(\x0b\x32#.openassetio_grpc_proto.TraitSetMsg\x12\x17\n\x0f\x65ntityReference\x18\x05 \x03(\t\"\x82\x02\n\x0fResolveResponse\x12O\n\rresultOrError\x18\x01 \x03(\x0b\x32\x38.openassetio_grpc_proto.ResolveResponse.ResultOrErrorMsg\x1a\x9d\x01\n\x10ResultOrErrorMsg\x12\r\n\x05index\x18\x01 \x01(\x04\x12\x37\n\x06result\x18\x02 \x01(\x0b\x32%.openassetio_grpc_proto.TraitsDataMsgH\x00\x12\x31\n\x05\x65rror\x18\x03 \x01(\x0b\x32 .openassetio_grpc_proto.ErrorMsgH\x00\x42\x0e\n\x0cresult_oneof\"\xe4\x01\n\x10PreflightRequest\x12\x0e\n\x06handle\x18\x01 \x01(\t\x12;\n\x0bhostSession\x18\x02 \x01(\x0b\x32&.openassetio_grpc_proto.HostSessionMsg\x12\x33\n\x07\x63ontext\x18\x03 \x01(\x0b\x32\".openassetio_grpc_proto.ContextMsg\x12\x35\n\x08traitSet\x18\x04 \x01(\x0b\x32#.openassetio_grpc_proto.TraitSetMsg\x12\x17\n\x0f\x65ntityReference\x18\x05 \x03(\t\"\xde\x01\n\x11PreflightResponse\x12Q\n\rresultOrError\x18\x01 \x03(\x0b\x32:.openassetio_grpc_proto.PreflightResponse.ResultOrErrorMsg\x1av\n\x10ResultOrErrorMsg\x12\r\n\x05index\x18\x01 \x01(\x04\x12\x10\n\x06result\x18\x02 \x01(\tH\x00\x12\x31\n\x05\x65rror\x18\x03 \x01(\x0b\x32 .openassetio_grpc_proto.ErrorMsgH\x00\x42\x0e\n\x0cresult_oneof\"\xe7\x01\n\x0fRegisterRequest\x12\x0e\n\x06handle\x18\x01 \x01(\t\x12;\n\x0bhostSession\x18\x02 \x01(\x0b\x32&.openassetio_grpc_proto.HostSessionMsg\x12\x33\n\x07\x63ontext\x18\x03 \x01(\x0b\x32\".openassetio_grpc_proto.ContextMsg\x12\x17\n\x0f\x65ntityReference\x18\x04 \x03(\t\x12\x39\n\ntraitsData\x18\x05 \x03(\x0b\x32%.openassetio_grpc_proto.TraitsDataMsg\"\xdc\x01\n\x10RegisterResponse\x12P\n\rresultOrError\x18\x01 \x03(\x0b\x32\x39.openassetio_grpc_proto.RegisterResponse.ResultOrErrorMsg\x1av\n\x10ResultOrErrorMsg\x12\r\n\x05index\x18\x01 \x01(\x04\x12\x10\n\x06result\x18\x02 \x01(\tH\x00\x12\x31\n\x05\x65rror\x18\x03 \x01(\x0b\x32 .openassetio_grpc_proto.ErrorMsgH\x00\x42\x0e\n\x0cresult_oneof2\xb2\n\n\x0cManagerProxy\x12`\n\x0bIdentifiers\x12$.openassetio_grpc_proto.EmptyRequest\x1a+.openassetio_grpc_proto.IdentifiersResponse\x12\x66\n\x0bInstantiate\x12*.openassetio_grpc_proto.InstantiateRequest\x1a+.openassetio_grpc_proto.InstantiateResponse\x12\x63\n\nIdentifier\x12).openassetio_grpc_proto.IdentifierRequest\x1a*.openassetio_grpc_proto.IdentifierResponse\x12\x66\n\x0b\x44isplayName\x12*.openassetio_grpc_proto.DisplayNameRequest\x1a+.openassetio_grpc_proto.DisplayNameResponse\x12Q\n\x04Info\x12#.openassetio_grpc_proto.InfoRequest\x1a$.openassetio_grpc_proto.InfoResponse\x12]\n\x08Settings\x12\'.openassetio_grpc_proto.SettingsRequest\x1a(.openassetio_grpc_proto.SettingsResponse\x12^\n\nInitialize\x12).openassetio_grpc_proto.InitializeRequest\x1a%.openassetio_grpc_proto.EmptyResponse\x12u\n\x10ManagementPolicy\x12/.openassetio_grpc_proto.ManagementPolicyRequest\x1a\x30.openassetio_grpc_proto.ManagementPolicyResponse\x12\x8a\x01\n\x17IsEntityReferenceString\x12\x36.openassetio_grpc_proto.IsEntityReferenceStringRequest\x1a\x37.openassetio_grpc_proto.IsEntityReferenceStringResponse\x12Z\n\x07Resolve\x12&.openassetio_grpc_proto.ResolveRequest\x1a\'.openassetio_grpc_proto.ResolveResponse\x12`\n\tPreflight\x12(.openassetio_grpc_proto.PreflightRequest\x1a).openassetio_grpc_proto.PreflightResponse\x12]\n\x08Register\x12\'.openassetio_grpc_proto.RegisterRequest\x1a(.openassetio_grpc_proto.RegisterResponse\x12X\n\x07\x44\x65stroy\x12&.openassetio_grpc_proto.DestroyRequest\x1a%.openassetio_grpc_proto.EmptyResponseb\x06proto3')

_globals = globals()
_builder.BuildMessageAndEnumDescriptors(DESCRIPTOR, _globals)
_builder.BuildTopDescriptorsAndMessages(DESCRIPTOR, 'openassetio_pb2', _globals)
if _descriptor._USE_C_DESCRIPTORS == False:

  DESCRIPTOR._options = None
  _INFODICTMSG_BOOLSENTRY._options = None
  _INFODICTMSG_BOOLSENTRY._serialized_options = b'8\001'
  _INFODICTMSG_INTSENTRY._options = None
  _INFODICTMSG_INTSENTRY._serialized_options = b'8\001'
  _INFODICTMSG_FLOATSENTRY._options = None
  _INFODICTMSG_FLOATSENTRY._serialized_options = b'8\001'
  _INFODICTMSG_STRINGSENTRY._options = None
  _INFODICTMSG_STRINGSENTRY._serialized_options = b'8\001'
  _TRAITSDATAMSG_TRAITMSG_BOOLSENTRY._options = None
  _TRAITSDATAMSG_TRAITMSG_BOOLSENTRY._serialized_options = b'8\001'
  _TRAITSDATAMSG_TRAITMSG_INTSENTRY._options = None
  _TRAITSDATAMSG_TRAITMSG_INTSENTRY._serialized_options = b'8\001'
  _TRAITSDATAMSG_TRAITMSG_FLOATSENTRY._options = None
  _TRAITSDATAMSG_TRAITMSG_FLOATSENTRY._serialized_options = b'8\001'
  _TRAITSDATAMSG_TRAITMSG_STRINGSENTRY._options = None
  _TRAITSDATAMSG_TRAITMSG_STRINGSENTRY._serialized_options = b'8\001'
  _globals['_EMPTYREQUEST']._serialized_start=45
  _globals['_EMPTYREQUEST']._serialized_end=59
  _globals['_EMPTYRESPONSE']._serialized_start=61
  _globals['_EMPTYRESPONSE']._serialized_end=76
  _globals['_ERRORMSG']._serialized_start=78
  _globals['_ERRORMSG']._serialized_end=124
  _globals['_INFODICTMSG']._serialized_start=127
  _globals['_INFODICTMSG']._serialized_end=582
  _globals['_INFODICTMSG_BOOLSENTRY']._serialized_start=398
  _globals['_INFODICTMSG_BOOLSENTRY']._serialized_end=442
  _globals['_INFODICTMSG_INTSENTRY']._serialized_start=444
  _globals['_INFODICTMSG_INTSENTRY']._serialized_end=487
  _globals['_INFODICTMSG_FLOATSENTRY']._serialized_start=489
  _globals['_INFODICTMSG_FLOATSENTRY']._serialized_end=534
  _globals['_INFODICTMSG_STRINGSENTRY']._serialized_start=536
  _globals['_INFODICTMSG_STRINGSENTRY']._serialized_end=582
  _globals['_HOSTMSG']._serialized_start=584
  _globals['_HOSTMSG']._serialized_end=634
  _globals['_HOSTSESSIONMSG']._serialized_start=636
  _globals['_HOSTSESSIONMSG']._serialized_end=699
  _globals['_CONTEXTMSG']._serialized_start=701
  _globals['_CONTEXTMSG']._serialized_end=803
  _globals['_TRAITSETMSG']._serialized_start=805
  _globals['_TRAITSETMSG']._serialized_end=835
  _globals['_TRAITSDATAMSG']._serialized_start=838
  _globals['_TRAITSDATAMSG']._serialized_end=1427
  _globals['_TRAITSDATAMSG_TRAITMSG']._serialized_start=919
  _globals['_TRAITSDATAMSG_TRAITMSG']._serialized_end=1427
  _globals['_TRAITSDATAMSG_TRAITMSG_BOOLSENTRY']._serialized_start=398
  _globals['_TRAITSDATAMSG_TRAITMSG_BOOLSENTRY']._serialized_end=442
  _globals['_TRAITSDATAMSG_TRAITMSG_INTSENTRY']._serialized_start=444
  _globals['_TRAITSDATAMSG_TRAITMSG_INTSENTRY']._serialized_end=487
  _globals['_TRAITSDATAMSG_TRAITMSG_FLOATSENTRY']._serialized_start=489
  _globals['_TRAITSDATAMSG_TRAITMSG_FLOATSENTRY']._serialized_end=534
  _globals['_TRAITSDATAMSG_TRAITMSG_STRINGSENTRY']._serialized_start=536
  _globals['_TRAITSDATAMSG_TRAITMSG_STRINGSENTRY']._serialized_end=582
  _globals['_IDENTIFIERSRESPONSE']._serialized_start=1429
  _globals['_IDENTIFIERSRESPONSE']._serialized_end=1470
  _globals['_INSTANTIATEREQUEST']._serialized_start=1472
  _globals['_INSTANTIATEREQUEST']._serialized_end=1512
  _globals['_INSTANTIATERESPONSE']._serialized_start=1514
  _globals['_INSTANTIATERESPONSE']._serialized_end=1551
  _globals['_DESTROYREQUEST']._serialized_start=1553
  _globals['_DESTROYREQUEST']._serialized_end=1585
  _globals['_IDENTIFIERREQUEST']._serialized_start=1587
  _globals['_IDENTIFIERREQUEST']._serialized_end=1622
  _globals['_IDENTIFIERRESPONSE']._serialized_start=1624
  _globals['_IDENTIFIERRESPONSE']._serialized_end=1664
  _globals['_DISPLAYNAMEREQUEST']._serialized_start=1666
  _globals['_DISPLAYNAMEREQUEST']._serialized_end=1702
  _globals['_DISPLAYNAMERESPONSE']._serialized_start=1704
  _globals['_DISPLAYNAMERESPONSE']._serialized_end=1746
  _globals['_INFOREQUEST']._serialized_start=1748
  _globals['_INFOREQUEST']._serialized_end=1777
  _globals['_INFORESPONSE']._serialized_start=1779
  _globals['_INFORESPONSE']._serialized_end=1844
  _globals['_SETTINGSREQUEST']._serialized_start=1846
  _globals['_SETTINGSREQUEST']._serialized_end=1940
  _globals['_SETTINGSRESPONSE']._serialized_start=1942
  _globals['_SETTINGSRESPONSE']._serialized_end=2015
  _globals['_INITIALIZEREQUEST']._serialized_start=2018
  _globals['_INITIALIZEREQUEST']._serialized_end=2169
  _globals['_MANAGEMENTPOLICYREQUEST']._serialized_start=2172
  _globals['_MANAGEMENTPOLICYREQUEST']._serialized_end=2382
  _globals['_MANAGEMENTPOLICYRESPONSE']._serialized_start=2384
  _globals['_MANAGEMENTPOLICYRESPONSE']._serialized_end=2465
  _globals['_ISENTITYREFERENCESTRINGREQUEST']._serialized_start=2468
  _globals['_ISENTITYREFERENCESTRINGREQUEST']._serialized_end=2597
  _globals['_ISENTITYREFERENCESTRINGRESPONSE']._serialized_start=2599
  _globals['_ISENTITYREFERENCESTRINGRESPONSE']._serialized_end=2644
  _globals['_RESOLVEREQUEST']._serialized_start=2647
  _globals['_RESOLVEREQUEST']._serialized_end=2873
  _globals['_RESOLVERESPONSE']._serialized_start=2876
  _globals['_RESOLVERESPONSE']._serialized_end=3134
  _globals['_RESOLVERESPONSE_RESULTORERRORMSG']._serialized_start=2977
  _globals['_RESOLVERESPONSE_RESULTORERRORMSG']._serialized_end=3134
  _globals['_PREFLIGHTREQUEST']._serialized_start=3137
  _globals['_PREFLIGHTREQUEST']._serialized_end=3365
  _globals['_PREFLIGHTRESPONSE']._serialized_start=3368
  _globals['_PREFLIGHTRESPONSE']._serialized_end=3590
  _globals['_PREFLIGHTRESPONSE_RESULTORERRORMSG']._serialized_start=3472
  _globals['_PREFLIGHTRESPONSE_RESULTORERRORMSG']._serialized_end=3590
  _globals['_REGISTERREQUEST']._serialized_start=3593
  _globals['_REGISTERREQUEST']._serialized_end=3824
  _globals['_REGISTERRESPONSE']._serialized_start=3827
  _globals['_REGISTERRESPONSE']._serialized_end=4047
  _globals['_REGISTERRESPONSE_RESULTORERRORMSG']._serialized_start=3472
  _globals['_REGISTERRESPONSE_RESULTORERRORMSG']._serialized_end=3590
  _globals['_MANAGERPROXY']._serialized_start=4050
  _globals['_MANAGERPROXY']._serialized_end=5380
# @@protoc_insertion_point(module_scope)
