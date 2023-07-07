from google.protobuf.internal import containers as _containers
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from typing import ClassVar as _ClassVar, Iterable as _Iterable, Mapping as _Mapping, Optional as _Optional, Union as _Union

DESCRIPTOR: _descriptor.FileDescriptor

class ContextMsg(_message.Message):
    __slots__ = ["access", "locale", "retention"]
    ACCESS_FIELD_NUMBER: _ClassVar[int]
    LOCALE_FIELD_NUMBER: _ClassVar[int]
    RETENTION_FIELD_NUMBER: _ClassVar[int]
    access: int
    locale: TraitsDataMsg
    retention: int
    def __init__(self, access: _Optional[int] = ..., retention: _Optional[int] = ..., locale: _Optional[_Union[TraitsDataMsg, _Mapping]] = ...) -> None: ...

class DestroyRequest(_message.Message):
    __slots__ = ["handle"]
    HANDLE_FIELD_NUMBER: _ClassVar[int]
    handle: str
    def __init__(self, handle: _Optional[str] = ...) -> None: ...

class DisplayNameRequest(_message.Message):
    __slots__ = ["handle"]
    HANDLE_FIELD_NUMBER: _ClassVar[int]
    handle: str
    def __init__(self, handle: _Optional[str] = ...) -> None: ...

class DisplayNameResponse(_message.Message):
    __slots__ = ["displayName"]
    DISPLAYNAME_FIELD_NUMBER: _ClassVar[int]
    displayName: str
    def __init__(self, displayName: _Optional[str] = ...) -> None: ...

class EmptyRequest(_message.Message):
    __slots__ = []
    def __init__(self) -> None: ...

class EmptyResponse(_message.Message):
    __slots__ = []
    def __init__(self) -> None: ...

class ErrorMsg(_message.Message):
    __slots__ = ["code", "errorMessage"]
    CODE_FIELD_NUMBER: _ClassVar[int]
    ERRORMESSAGE_FIELD_NUMBER: _ClassVar[int]
    code: int
    errorMessage: str
    def __init__(self, code: _Optional[int] = ..., errorMessage: _Optional[str] = ...) -> None: ...

class HostMsg(_message.Message):
    __slots__ = ["displayName", "identifier"]
    DISPLAYNAME_FIELD_NUMBER: _ClassVar[int]
    IDENTIFIER_FIELD_NUMBER: _ClassVar[int]
    displayName: str
    identifier: str
    def __init__(self, identifier: _Optional[str] = ..., displayName: _Optional[str] = ...) -> None: ...

class HostSessionMsg(_message.Message):
    __slots__ = ["host"]
    HOST_FIELD_NUMBER: _ClassVar[int]
    host: HostMsg
    def __init__(self, host: _Optional[_Union[HostMsg, _Mapping]] = ...) -> None: ...

class IdentifierRequest(_message.Message):
    __slots__ = ["handle"]
    HANDLE_FIELD_NUMBER: _ClassVar[int]
    handle: str
    def __init__(self, handle: _Optional[str] = ...) -> None: ...

class IdentifierResponse(_message.Message):
    __slots__ = ["identifier"]
    IDENTIFIER_FIELD_NUMBER: _ClassVar[int]
    identifier: str
    def __init__(self, identifier: _Optional[str] = ...) -> None: ...

class IdentifiersResponse(_message.Message):
    __slots__ = ["identifier"]
    IDENTIFIER_FIELD_NUMBER: _ClassVar[int]
    identifier: _containers.RepeatedScalarFieldContainer[str]
    def __init__(self, identifier: _Optional[_Iterable[str]] = ...) -> None: ...

class InfoDictMsg(_message.Message):
    __slots__ = ["bools", "floats", "ints", "strings"]
    class BoolsEntry(_message.Message):
        __slots__ = ["key", "value"]
        KEY_FIELD_NUMBER: _ClassVar[int]
        VALUE_FIELD_NUMBER: _ClassVar[int]
        key: str
        value: bool
        def __init__(self, key: _Optional[str] = ..., value: bool = ...) -> None: ...
    class FloatsEntry(_message.Message):
        __slots__ = ["key", "value"]
        KEY_FIELD_NUMBER: _ClassVar[int]
        VALUE_FIELD_NUMBER: _ClassVar[int]
        key: str
        value: float
        def __init__(self, key: _Optional[str] = ..., value: _Optional[float] = ...) -> None: ...
    class IntsEntry(_message.Message):
        __slots__ = ["key", "value"]
        KEY_FIELD_NUMBER: _ClassVar[int]
        VALUE_FIELD_NUMBER: _ClassVar[int]
        key: str
        value: int
        def __init__(self, key: _Optional[str] = ..., value: _Optional[int] = ...) -> None: ...
    class StringsEntry(_message.Message):
        __slots__ = ["key", "value"]
        KEY_FIELD_NUMBER: _ClassVar[int]
        VALUE_FIELD_NUMBER: _ClassVar[int]
        key: str
        value: str
        def __init__(self, key: _Optional[str] = ..., value: _Optional[str] = ...) -> None: ...
    BOOLS_FIELD_NUMBER: _ClassVar[int]
    FLOATS_FIELD_NUMBER: _ClassVar[int]
    INTS_FIELD_NUMBER: _ClassVar[int]
    STRINGS_FIELD_NUMBER: _ClassVar[int]
    bools: _containers.ScalarMap[str, bool]
    floats: _containers.ScalarMap[str, float]
    ints: _containers.ScalarMap[str, int]
    strings: _containers.ScalarMap[str, str]
    def __init__(self, bools: _Optional[_Mapping[str, bool]] = ..., ints: _Optional[_Mapping[str, int]] = ..., floats: _Optional[_Mapping[str, float]] = ..., strings: _Optional[_Mapping[str, str]] = ...) -> None: ...

class InfoRequest(_message.Message):
    __slots__ = ["handle"]
    HANDLE_FIELD_NUMBER: _ClassVar[int]
    handle: str
    def __init__(self, handle: _Optional[str] = ...) -> None: ...

class InfoResponse(_message.Message):
    __slots__ = ["info"]
    INFO_FIELD_NUMBER: _ClassVar[int]
    info: InfoDictMsg
    def __init__(self, info: _Optional[_Union[InfoDictMsg, _Mapping]] = ...) -> None: ...

class InitializeRequest(_message.Message):
    __slots__ = ["handle", "hostSession", "settings"]
    HANDLE_FIELD_NUMBER: _ClassVar[int]
    HOSTSESSION_FIELD_NUMBER: _ClassVar[int]
    SETTINGS_FIELD_NUMBER: _ClassVar[int]
    handle: str
    hostSession: HostSessionMsg
    settings: InfoDictMsg
    def __init__(self, handle: _Optional[str] = ..., hostSession: _Optional[_Union[HostSessionMsg, _Mapping]] = ..., settings: _Optional[_Union[InfoDictMsg, _Mapping]] = ...) -> None: ...

class InstantiateRequest(_message.Message):
    __slots__ = ["identifier"]
    IDENTIFIER_FIELD_NUMBER: _ClassVar[int]
    identifier: str
    def __init__(self, identifier: _Optional[str] = ...) -> None: ...

class InstantiateResponse(_message.Message):
    __slots__ = ["handle"]
    HANDLE_FIELD_NUMBER: _ClassVar[int]
    handle: str
    def __init__(self, handle: _Optional[str] = ...) -> None: ...

class IsEntityReferenceStringRequest(_message.Message):
    __slots__ = ["handle", "hostSession", "someString"]
    HANDLE_FIELD_NUMBER: _ClassVar[int]
    HOSTSESSION_FIELD_NUMBER: _ClassVar[int]
    SOMESTRING_FIELD_NUMBER: _ClassVar[int]
    handle: str
    hostSession: HostSessionMsg
    someString: str
    def __init__(self, handle: _Optional[str] = ..., hostSession: _Optional[_Union[HostSessionMsg, _Mapping]] = ..., someString: _Optional[str] = ...) -> None: ...

class IsEntityReferenceStringResponse(_message.Message):
    __slots__ = []
    IS_FIELD_NUMBER: _ClassVar[int]
    def __init__(self, **kwargs) -> None: ...

class ManagementPolicyRequest(_message.Message):
    __slots__ = ["context", "handle", "hostSession", "traitSet"]
    CONTEXT_FIELD_NUMBER: _ClassVar[int]
    HANDLE_FIELD_NUMBER: _ClassVar[int]
    HOSTSESSION_FIELD_NUMBER: _ClassVar[int]
    TRAITSET_FIELD_NUMBER: _ClassVar[int]
    context: ContextMsg
    handle: str
    hostSession: HostSessionMsg
    traitSet: _containers.RepeatedCompositeFieldContainer[TraitSetMsg]
    def __init__(self, handle: _Optional[str] = ..., hostSession: _Optional[_Union[HostSessionMsg, _Mapping]] = ..., context: _Optional[_Union[ContextMsg, _Mapping]] = ..., traitSet: _Optional[_Iterable[_Union[TraitSetMsg, _Mapping]]] = ...) -> None: ...

class ManagementPolicyResponse(_message.Message):
    __slots__ = ["policy"]
    POLICY_FIELD_NUMBER: _ClassVar[int]
    policy: _containers.RepeatedCompositeFieldContainer[TraitsDataMsg]
    def __init__(self, policy: _Optional[_Iterable[_Union[TraitsDataMsg, _Mapping]]] = ...) -> None: ...

class PreflightRequest(_message.Message):
    __slots__ = ["context", "entityReference", "handle", "hostSession", "traitSet"]
    CONTEXT_FIELD_NUMBER: _ClassVar[int]
    ENTITYREFERENCE_FIELD_NUMBER: _ClassVar[int]
    HANDLE_FIELD_NUMBER: _ClassVar[int]
    HOSTSESSION_FIELD_NUMBER: _ClassVar[int]
    TRAITSET_FIELD_NUMBER: _ClassVar[int]
    context: ContextMsg
    entityReference: _containers.RepeatedScalarFieldContainer[str]
    handle: str
    hostSession: HostSessionMsg
    traitSet: TraitSetMsg
    def __init__(self, handle: _Optional[str] = ..., hostSession: _Optional[_Union[HostSessionMsg, _Mapping]] = ..., context: _Optional[_Union[ContextMsg, _Mapping]] = ..., traitSet: _Optional[_Union[TraitSetMsg, _Mapping]] = ..., entityReference: _Optional[_Iterable[str]] = ...) -> None: ...

class PreflightResponse(_message.Message):
    __slots__ = ["resultOrError"]
    class ResultOrErrorMsg(_message.Message):
        __slots__ = ["error", "index", "result"]
        ERROR_FIELD_NUMBER: _ClassVar[int]
        INDEX_FIELD_NUMBER: _ClassVar[int]
        RESULT_FIELD_NUMBER: _ClassVar[int]
        error: ErrorMsg
        index: int
        result: str
        def __init__(self, index: _Optional[int] = ..., result: _Optional[str] = ..., error: _Optional[_Union[ErrorMsg, _Mapping]] = ...) -> None: ...
    RESULTORERROR_FIELD_NUMBER: _ClassVar[int]
    resultOrError: _containers.RepeatedCompositeFieldContainer[PreflightResponse.ResultOrErrorMsg]
    def __init__(self, resultOrError: _Optional[_Iterable[_Union[PreflightResponse.ResultOrErrorMsg, _Mapping]]] = ...) -> None: ...

class RegisterRequest(_message.Message):
    __slots__ = ["context", "entityReference", "handle", "hostSession", "traitsData"]
    CONTEXT_FIELD_NUMBER: _ClassVar[int]
    ENTITYREFERENCE_FIELD_NUMBER: _ClassVar[int]
    HANDLE_FIELD_NUMBER: _ClassVar[int]
    HOSTSESSION_FIELD_NUMBER: _ClassVar[int]
    TRAITSDATA_FIELD_NUMBER: _ClassVar[int]
    context: ContextMsg
    entityReference: _containers.RepeatedScalarFieldContainer[str]
    handle: str
    hostSession: HostSessionMsg
    traitsData: _containers.RepeatedCompositeFieldContainer[TraitsDataMsg]
    def __init__(self, handle: _Optional[str] = ..., hostSession: _Optional[_Union[HostSessionMsg, _Mapping]] = ..., context: _Optional[_Union[ContextMsg, _Mapping]] = ..., entityReference: _Optional[_Iterable[str]] = ..., traitsData: _Optional[_Iterable[_Union[TraitsDataMsg, _Mapping]]] = ...) -> None: ...

class RegisterResponse(_message.Message):
    __slots__ = ["resultOrError"]
    class ResultOrErrorMsg(_message.Message):
        __slots__ = ["error", "index", "result"]
        ERROR_FIELD_NUMBER: _ClassVar[int]
        INDEX_FIELD_NUMBER: _ClassVar[int]
        RESULT_FIELD_NUMBER: _ClassVar[int]
        error: ErrorMsg
        index: int
        result: str
        def __init__(self, index: _Optional[int] = ..., result: _Optional[str] = ..., error: _Optional[_Union[ErrorMsg, _Mapping]] = ...) -> None: ...
    RESULTORERROR_FIELD_NUMBER: _ClassVar[int]
    resultOrError: _containers.RepeatedCompositeFieldContainer[RegisterResponse.ResultOrErrorMsg]
    def __init__(self, resultOrError: _Optional[_Iterable[_Union[RegisterResponse.ResultOrErrorMsg, _Mapping]]] = ...) -> None: ...

class ResolveRequest(_message.Message):
    __slots__ = ["context", "entityReference", "handle", "hostSession", "traitSet"]
    CONTEXT_FIELD_NUMBER: _ClassVar[int]
    ENTITYREFERENCE_FIELD_NUMBER: _ClassVar[int]
    HANDLE_FIELD_NUMBER: _ClassVar[int]
    HOSTSESSION_FIELD_NUMBER: _ClassVar[int]
    TRAITSET_FIELD_NUMBER: _ClassVar[int]
    context: ContextMsg
    entityReference: _containers.RepeatedScalarFieldContainer[str]
    handle: str
    hostSession: HostSessionMsg
    traitSet: TraitSetMsg
    def __init__(self, handle: _Optional[str] = ..., hostSession: _Optional[_Union[HostSessionMsg, _Mapping]] = ..., context: _Optional[_Union[ContextMsg, _Mapping]] = ..., traitSet: _Optional[_Union[TraitSetMsg, _Mapping]] = ..., entityReference: _Optional[_Iterable[str]] = ...) -> None: ...

class ResolveResponse(_message.Message):
    __slots__ = ["resultOrError"]
    class ResultOrErrorMsg(_message.Message):
        __slots__ = ["error", "index", "result"]
        ERROR_FIELD_NUMBER: _ClassVar[int]
        INDEX_FIELD_NUMBER: _ClassVar[int]
        RESULT_FIELD_NUMBER: _ClassVar[int]
        error: ErrorMsg
        index: int
        result: TraitsDataMsg
        def __init__(self, index: _Optional[int] = ..., result: _Optional[_Union[TraitsDataMsg, _Mapping]] = ..., error: _Optional[_Union[ErrorMsg, _Mapping]] = ...) -> None: ...
    RESULTORERROR_FIELD_NUMBER: _ClassVar[int]
    resultOrError: _containers.RepeatedCompositeFieldContainer[ResolveResponse.ResultOrErrorMsg]
    def __init__(self, resultOrError: _Optional[_Iterable[_Union[ResolveResponse.ResultOrErrorMsg, _Mapping]]] = ...) -> None: ...

class TraitSetMsg(_message.Message):
    __slots__ = ["traitId"]
    TRAITID_FIELD_NUMBER: _ClassVar[int]
    traitId: _containers.RepeatedScalarFieldContainer[str]
    def __init__(self, traitId: _Optional[_Iterable[str]] = ...) -> None: ...

class TraitsDataMsg(_message.Message):
    __slots__ = ["trait"]
    class TraitMsg(_message.Message):
        __slots__ = ["bools", "floats", "id", "ints", "strings"]
        class BoolsEntry(_message.Message):
            __slots__ = ["key", "value"]
            KEY_FIELD_NUMBER: _ClassVar[int]
            VALUE_FIELD_NUMBER: _ClassVar[int]
            key: str
            value: bool
            def __init__(self, key: _Optional[str] = ..., value: bool = ...) -> None: ...
        class FloatsEntry(_message.Message):
            __slots__ = ["key", "value"]
            KEY_FIELD_NUMBER: _ClassVar[int]
            VALUE_FIELD_NUMBER: _ClassVar[int]
            key: str
            value: float
            def __init__(self, key: _Optional[str] = ..., value: _Optional[float] = ...) -> None: ...
        class IntsEntry(_message.Message):
            __slots__ = ["key", "value"]
            KEY_FIELD_NUMBER: _ClassVar[int]
            VALUE_FIELD_NUMBER: _ClassVar[int]
            key: str
            value: int
            def __init__(self, key: _Optional[str] = ..., value: _Optional[int] = ...) -> None: ...
        class StringsEntry(_message.Message):
            __slots__ = ["key", "value"]
            KEY_FIELD_NUMBER: _ClassVar[int]
            VALUE_FIELD_NUMBER: _ClassVar[int]
            key: str
            value: str
            def __init__(self, key: _Optional[str] = ..., value: _Optional[str] = ...) -> None: ...
        BOOLS_FIELD_NUMBER: _ClassVar[int]
        FLOATS_FIELD_NUMBER: _ClassVar[int]
        ID_FIELD_NUMBER: _ClassVar[int]
        INTS_FIELD_NUMBER: _ClassVar[int]
        STRINGS_FIELD_NUMBER: _ClassVar[int]
        bools: _containers.ScalarMap[str, bool]
        floats: _containers.ScalarMap[str, float]
        id: str
        ints: _containers.ScalarMap[str, int]
        strings: _containers.ScalarMap[str, str]
        def __init__(self, id: _Optional[str] = ..., bools: _Optional[_Mapping[str, bool]] = ..., ints: _Optional[_Mapping[str, int]] = ..., floats: _Optional[_Mapping[str, float]] = ..., strings: _Optional[_Mapping[str, str]] = ...) -> None: ...
    TRAIT_FIELD_NUMBER: _ClassVar[int]
    trait: _containers.RepeatedCompositeFieldContainer[TraitsDataMsg.TraitMsg]
    def __init__(self, trait: _Optional[_Iterable[_Union[TraitsDataMsg.TraitMsg, _Mapping]]] = ...) -> None: ...
