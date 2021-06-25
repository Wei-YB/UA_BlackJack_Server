// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: demo.proto

#include "demo.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>

PROTOBUF_PRAGMA_INIT_SEG
namespace ua_blackjack {
constexpr Request::Request(
  ::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized)
  : args_()
  , uid_(PROTOBUF_LONGLONG(0))
  , stamp_(PROTOBUF_LONGLONG(0))
  , requesttype_(0)
{}
struct RequestDefaultTypeInternal {
  constexpr RequestDefaultTypeInternal()
    : _instance(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized{}) {}
  ~RequestDefaultTypeInternal() {}
  union {
    Request _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT RequestDefaultTypeInternal _Request_default_instance_;
constexpr Response::Response(
  ::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized)
  : args_()
  , status_(PROTOBUF_LONGLONG(0))
  , uid_(PROTOBUF_LONGLONG(0))
  , stamp_(PROTOBUF_LONGLONG(0)){}
struct ResponseDefaultTypeInternal {
  constexpr ResponseDefaultTypeInternal()
    : _instance(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized{}) {}
  ~ResponseDefaultTypeInternal() {}
  union {
    Response _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT ResponseDefaultTypeInternal _Response_default_instance_;
}  // namespace ua_blackjack
static ::PROTOBUF_NAMESPACE_ID::Metadata file_level_metadata_demo_2eproto[2];
static const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* file_level_enum_descriptors_demo_2eproto[1];
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_demo_2eproto = nullptr;

const ::PROTOBUF_NAMESPACE_ID::uint32 TableStruct_demo_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  PROTOBUF_FIELD_OFFSET(::ua_blackjack::Request, _has_bits_),
  PROTOBUF_FIELD_OFFSET(::ua_blackjack::Request, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::ua_blackjack::Request, requesttype_),
  PROTOBUF_FIELD_OFFSET(::ua_blackjack::Request, uid_),
  PROTOBUF_FIELD_OFFSET(::ua_blackjack::Request, stamp_),
  PROTOBUF_FIELD_OFFSET(::ua_blackjack::Request, args_),
  2,
  0,
  1,
  ~0u,
  PROTOBUF_FIELD_OFFSET(::ua_blackjack::Response, _has_bits_),
  PROTOBUF_FIELD_OFFSET(::ua_blackjack::Response, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::ua_blackjack::Response, status_),
  PROTOBUF_FIELD_OFFSET(::ua_blackjack::Response, uid_),
  PROTOBUF_FIELD_OFFSET(::ua_blackjack::Response, stamp_),
  PROTOBUF_FIELD_OFFSET(::ua_blackjack::Response, args_),
  0,
  1,
  2,
  ~0u,
};
static const ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, 9, sizeof(::ua_blackjack::Request)},
  { 13, 22, sizeof(::ua_blackjack::Response)},
};

static ::PROTOBUF_NAMESPACE_ID::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::ua_blackjack::_Request_default_instance_),
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::ua_blackjack::_Response_default_instance_),
};

const char descriptor_table_protodef_demo_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\ndemo.proto\022\014ua_blackjack\"\251\005\n\007Request\022;"
  "\n\013requestType\030\001 \001(\0162!.ua_blackjack.Reque"
  "st.RequestTypeH\000\210\001\001\022\020\n\003uid\030\002 \001(\003H\001\210\001\001\022\022\n"
  "\005stamp\030\003 \001(\003H\002\210\001\001\022\014\n\004args\030\004 \003(\t\"\212\004\n\013Requ"
  "estType\022\t\n\005INVAL\020\000\022\t\n\005LOGIN\020\001\022\n\n\006LOGOUT\020"
  "\002\022\r\n\tROOM_LIST\020\003\022\r\n\tJOIN_ROOM\020\004\022\017\n\013CREAT"
  "E_ROOM\020\005\022\017\n\013QUICK_MATCH\020\006\022\t\n\005READY\020\007\022\016\n\n"
  "LEAVE_ROOM\020\010\022\007\n\003BET\020\t\022\007\n\003HIT\020\n\022\t\n\005STAND\020"
  "\013\022\n\n\006DOUBLE\020\014\022\r\n\tSURRENDER\020\r\022\n\n\006SIGNUP\020\016"
  "\022\010\n\004INFO\020\017\022\013\n\007RANK_ME\020\020\022\014\n\010RANK_TOP\020\021\022\016\n"
  "\nADD_FRIEND\020\022\022\021\n\rACCEPT_FRIEND\020\023\022\021\n\rDELE"
  "TE_FRIEND\020\024\022\017\n\013LIST_FRIEND\020\025\022\016\n\nLIST_MAT"
  "CH\020\026\022\021\n\rLIST_WAITTING\020\027\022\017\n\013NOTIFY_USER\020\030"
  "\022\014\n\010GET_NAME\020\031\022\013\n\007GET_UID\020\032\022\020\n\014GET_PASSW"
  "ORD\020\033\022\r\n\tGET_SCORE\020\034\022\022\n\016GET_MATCH_INFO\020\035"
  "\022\r\n\tMATCH_END\020\036\022\023\n\017ADD_WAIT_FRIEND\020\037\022\026\n\022"
  "DELETE_WAIT_FRIEND\020 \022\016\n\nGAME_START\020!B\016\n\014"
  "_requestTypeB\006\n\004_uidB\010\n\006_stamp\"p\n\010Respon"
  "se\022\023\n\006status\030\001 \001(\003H\000\210\001\001\022\020\n\003uid\030\002 \001(\003H\001\210\001"
  "\001\022\022\n\005stamp\030\003 \001(\003H\002\210\001\001\022\014\n\004args\030\004 \003(\tB\t\n\007_"
  "statusB\006\n\004_uidB\010\n\006_stamp2I\n\014LobbyService"
  "\0229\n\006Notify\022\025.ua_blackjack.Request\032\026.ua_b"
  "lackjack.Response\"\0002H\n\013GameService\0229\n\006No"
  "tify\022\025.ua_blackjack.Request\032\026.ua_blackja"
  "ck.Response\"\0002J\n\rSocialService\0229\n\006Notify"
  "\022\025.ua_blackjack.Request\032\026.ua_blackjack.R"
  "esponse\"\0002J\n\rPlayerService\0229\n\006Notify\022\025.u"
  "a_blackjack.Request\032\026.ua_blackjack.Respo"
  "nse\"\0002L\n\017DatabaseService\0229\n\006Notify\022\025.ua_"
  "blackjack.Request\032\026.ua_blackjack.Respons"
  "e\"\0002I\n\014ProxyService\0229\n\006Notify\022\025.ua_black"
  "jack.Request\032\026.ua_blackjack.Response\"\0002H"
  "\n\013UserService\0229\n\006Notify\022\025.ua_blackjack.R"
  "equest\032\026.ua_blackjack.Response\"\000b\006proto3"
  ;
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_demo_2eproto_once;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_demo_2eproto = {
  false, false, 1360, descriptor_table_protodef_demo_2eproto, "demo.proto", 
  &descriptor_table_demo_2eproto_once, nullptr, 0, 2,
  schemas, file_default_instances, TableStruct_demo_2eproto::offsets,
  file_level_metadata_demo_2eproto, file_level_enum_descriptors_demo_2eproto, file_level_service_descriptors_demo_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK ::PROTOBUF_NAMESPACE_ID::Metadata
descriptor_table_demo_2eproto_metadata_getter(int index) {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_demo_2eproto);
  return descriptor_table_demo_2eproto.file_level_metadata[index];
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY static ::PROTOBUF_NAMESPACE_ID::internal::AddDescriptorsRunner dynamic_init_dummy_demo_2eproto(&descriptor_table_demo_2eproto);
namespace ua_blackjack {
const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* Request_RequestType_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_demo_2eproto);
  return file_level_enum_descriptors_demo_2eproto[0];
}
bool Request_RequestType_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
    case 22:
    case 23:
    case 24:
    case 25:
    case 26:
    case 27:
    case 28:
    case 29:
    case 30:
    case 31:
    case 32:
    case 33:
      return true;
    default:
      return false;
  }
}

#if (__cplusplus < 201703) && (!defined(_MSC_VER) || _MSC_VER >= 1900)
constexpr Request_RequestType Request::INVAL;
constexpr Request_RequestType Request::LOGIN;
constexpr Request_RequestType Request::LOGOUT;
constexpr Request_RequestType Request::ROOM_LIST;
constexpr Request_RequestType Request::JOIN_ROOM;
constexpr Request_RequestType Request::CREATE_ROOM;
constexpr Request_RequestType Request::QUICK_MATCH;
constexpr Request_RequestType Request::READY;
constexpr Request_RequestType Request::LEAVE_ROOM;
constexpr Request_RequestType Request::BET;
constexpr Request_RequestType Request::HIT;
constexpr Request_RequestType Request::STAND;
constexpr Request_RequestType Request::DOUBLE;
constexpr Request_RequestType Request::SURRENDER;
constexpr Request_RequestType Request::SIGNUP;
constexpr Request_RequestType Request::INFO;
constexpr Request_RequestType Request::RANK_ME;
constexpr Request_RequestType Request::RANK_TOP;
constexpr Request_RequestType Request::ADD_FRIEND;
constexpr Request_RequestType Request::ACCEPT_FRIEND;
constexpr Request_RequestType Request::DELETE_FRIEND;
constexpr Request_RequestType Request::LIST_FRIEND;
constexpr Request_RequestType Request::LIST_MATCH;
constexpr Request_RequestType Request::LIST_WAITTING;
constexpr Request_RequestType Request::NOTIFY_USER;
constexpr Request_RequestType Request::GET_NAME;
constexpr Request_RequestType Request::GET_UID;
constexpr Request_RequestType Request::GET_PASSWORD;
constexpr Request_RequestType Request::GET_SCORE;
constexpr Request_RequestType Request::GET_MATCH_INFO;
constexpr Request_RequestType Request::MATCH_END;
constexpr Request_RequestType Request::ADD_WAIT_FRIEND;
constexpr Request_RequestType Request::DELETE_WAIT_FRIEND;
constexpr Request_RequestType Request::GAME_START;
constexpr Request_RequestType Request::RequestType_MIN;
constexpr Request_RequestType Request::RequestType_MAX;
constexpr int Request::RequestType_ARRAYSIZE;
#endif  // (__cplusplus < 201703) && (!defined(_MSC_VER) || _MSC_VER >= 1900)

// ===================================================================

class Request::_Internal {
 public:
  using HasBits = decltype(std::declval<Request>()._has_bits_);
  static void set_has_requesttype(HasBits* has_bits) {
    (*has_bits)[0] |= 4u;
  }
  static void set_has_uid(HasBits* has_bits) {
    (*has_bits)[0] |= 1u;
  }
  static void set_has_stamp(HasBits* has_bits) {
    (*has_bits)[0] |= 2u;
  }
};

Request::Request(::PROTOBUF_NAMESPACE_ID::Arena* arena)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena),
  args_(arena) {
  SharedCtor();
  RegisterArenaDtor(arena);
  // @@protoc_insertion_point(arena_constructor:ua_blackjack.Request)
}
Request::Request(const Request& from)
  : ::PROTOBUF_NAMESPACE_ID::Message(),
      _has_bits_(from._has_bits_),
      args_(from.args_) {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::memcpy(&uid_, &from.uid_,
    static_cast<size_t>(reinterpret_cast<char*>(&requesttype_) -
    reinterpret_cast<char*>(&uid_)) + sizeof(requesttype_));
  // @@protoc_insertion_point(copy_constructor:ua_blackjack.Request)
}

void Request::SharedCtor() {
::memset(reinterpret_cast<char*>(this) + static_cast<size_t>(
    reinterpret_cast<char*>(&uid_) - reinterpret_cast<char*>(this)),
    0, static_cast<size_t>(reinterpret_cast<char*>(&requesttype_) -
    reinterpret_cast<char*>(&uid_)) + sizeof(requesttype_));
}

Request::~Request() {
  // @@protoc_insertion_point(destructor:ua_blackjack.Request)
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

void Request::SharedDtor() {
  GOOGLE_DCHECK(GetArena() == nullptr);
}

void Request::ArenaDtor(void* object) {
  Request* _this = reinterpret_cast< Request* >(object);
  (void)_this;
}
void Request::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void Request::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}

void Request::Clear() {
// @@protoc_insertion_point(message_clear_start:ua_blackjack.Request)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  args_.Clear();
  cached_has_bits = _has_bits_[0];
  if (cached_has_bits & 0x00000007u) {
    ::memset(&uid_, 0, static_cast<size_t>(
        reinterpret_cast<char*>(&requesttype_) -
        reinterpret_cast<char*>(&uid_)) + sizeof(requesttype_));
  }
  _has_bits_.Clear();
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* Request::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  _Internal::HasBits has_bits{};
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    CHK_(ptr);
    switch (tag >> 3) {
      // .ua_blackjack.Request.RequestType requestType = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 8)) {
          ::PROTOBUF_NAMESPACE_ID::uint64 val = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
          _internal_set_requesttype(static_cast<::ua_blackjack::Request_RequestType>(val));
        } else goto handle_unusual;
        continue;
      // int64 uid = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 16)) {
          _Internal::set_has_uid(&has_bits);
          uid_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // int64 stamp = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 24)) {
          _Internal::set_has_stamp(&has_bits);
          stamp_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // repeated string args = 4;
      case 4:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 34)) {
          ptr -= 1;
          do {
            ptr += 1;
            auto str = _internal_add_args();
            ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParser(str, ptr, ctx);
            CHK_(::PROTOBUF_NAMESPACE_ID::internal::VerifyUTF8(str, "ua_blackjack.Request.args"));
            CHK_(ptr);
            if (!ctx->DataAvailable(ptr)) break;
          } while (::PROTOBUF_NAMESPACE_ID::internal::ExpectTag<34>(ptr));
        } else goto handle_unusual;
        continue;
      default: {
      handle_unusual:
        if ((tag & 7) == 4 || tag == 0) {
          ctx->SetLastTag(tag);
          goto success;
        }
        ptr = UnknownFieldParse(tag,
            _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
            ptr, ctx);
        CHK_(ptr != nullptr);
        continue;
      }
    }  // switch
  }  // while
success:
  _has_bits_.Or(has_bits);
  return ptr;
failure:
  ptr = nullptr;
  goto success;
#undef CHK_
}

::PROTOBUF_NAMESPACE_ID::uint8* Request::_InternalSerialize(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:ua_blackjack.Request)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // .ua_blackjack.Request.RequestType requestType = 1;
  if (_internal_has_requesttype()) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteEnumToArray(
      1, this->_internal_requesttype(), target);
  }

  // int64 uid = 2;
  if (_internal_has_uid()) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt64ToArray(2, this->_internal_uid(), target);
  }

  // int64 stamp = 3;
  if (_internal_has_stamp()) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt64ToArray(3, this->_internal_stamp(), target);
  }

  // repeated string args = 4;
  for (int i = 0, n = this->_internal_args_size(); i < n; i++) {
    const auto& s = this->_internal_args(i);
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      s.data(), static_cast<int>(s.length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "ua_blackjack.Request.args");
    target = stream->WriteString(4, s, target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:ua_blackjack.Request)
  return target;
}

size_t Request::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:ua_blackjack.Request)
  size_t total_size = 0;

  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // repeated string args = 4;
  total_size += 1 *
      ::PROTOBUF_NAMESPACE_ID::internal::FromIntSize(args_.size());
  for (int i = 0, n = args_.size(); i < n; i++) {
    total_size += ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
      args_.Get(i));
  }

  cached_has_bits = _has_bits_[0];
  if (cached_has_bits & 0x00000007u) {
    // int64 uid = 2;
    if (cached_has_bits & 0x00000001u) {
      total_size += 1 +
        ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int64Size(
          this->_internal_uid());
    }

    // int64 stamp = 3;
    if (cached_has_bits & 0x00000002u) {
      total_size += 1 +
        ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int64Size(
          this->_internal_stamp());
    }

    // .ua_blackjack.Request.RequestType requestType = 1;
    if (cached_has_bits & 0x00000004u) {
      total_size += 1 +
        ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::EnumSize(this->_internal_requesttype());
    }

  }
  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void Request::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:ua_blackjack.Request)
  GOOGLE_DCHECK_NE(&from, this);
  const Request* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<Request>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:ua_blackjack.Request)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:ua_blackjack.Request)
    MergeFrom(*source);
  }
}

void Request::MergeFrom(const Request& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:ua_blackjack.Request)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  args_.MergeFrom(from.args_);
  cached_has_bits = from._has_bits_[0];
  if (cached_has_bits & 0x00000007u) {
    if (cached_has_bits & 0x00000001u) {
      uid_ = from.uid_;
    }
    if (cached_has_bits & 0x00000002u) {
      stamp_ = from.stamp_;
    }
    if (cached_has_bits & 0x00000004u) {
      requesttype_ = from.requesttype_;
    }
    _has_bits_[0] |= cached_has_bits;
  }
}

void Request::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:ua_blackjack.Request)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Request::CopyFrom(const Request& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:ua_blackjack.Request)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Request::IsInitialized() const {
  return true;
}

void Request::InternalSwap(Request* other) {
  using std::swap;
  _internal_metadata_.Swap<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(&other->_internal_metadata_);
  swap(_has_bits_[0], other->_has_bits_[0]);
  args_.InternalSwap(&other->args_);
  ::PROTOBUF_NAMESPACE_ID::internal::memswap<
      PROTOBUF_FIELD_OFFSET(Request, requesttype_)
      + sizeof(Request::requesttype_)
      - PROTOBUF_FIELD_OFFSET(Request, uid_)>(
          reinterpret_cast<char*>(&uid_),
          reinterpret_cast<char*>(&other->uid_));
}

::PROTOBUF_NAMESPACE_ID::Metadata Request::GetMetadata() const {
  return GetMetadataStatic();
}


// ===================================================================

class Response::_Internal {
 public:
  using HasBits = decltype(std::declval<Response>()._has_bits_);
  static void set_has_status(HasBits* has_bits) {
    (*has_bits)[0] |= 1u;
  }
  static void set_has_uid(HasBits* has_bits) {
    (*has_bits)[0] |= 2u;
  }
  static void set_has_stamp(HasBits* has_bits) {
    (*has_bits)[0] |= 4u;
  }
};

Response::Response(::PROTOBUF_NAMESPACE_ID::Arena* arena)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena),
  args_(arena) {
  SharedCtor();
  RegisterArenaDtor(arena);
  // @@protoc_insertion_point(arena_constructor:ua_blackjack.Response)
}
Response::Response(const Response& from)
  : ::PROTOBUF_NAMESPACE_ID::Message(),
      _has_bits_(from._has_bits_),
      args_(from.args_) {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::memcpy(&status_, &from.status_,
    static_cast<size_t>(reinterpret_cast<char*>(&stamp_) -
    reinterpret_cast<char*>(&status_)) + sizeof(stamp_));
  // @@protoc_insertion_point(copy_constructor:ua_blackjack.Response)
}

void Response::SharedCtor() {
::memset(reinterpret_cast<char*>(this) + static_cast<size_t>(
    reinterpret_cast<char*>(&status_) - reinterpret_cast<char*>(this)),
    0, static_cast<size_t>(reinterpret_cast<char*>(&stamp_) -
    reinterpret_cast<char*>(&status_)) + sizeof(stamp_));
}

Response::~Response() {
  // @@protoc_insertion_point(destructor:ua_blackjack.Response)
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

void Response::SharedDtor() {
  GOOGLE_DCHECK(GetArena() == nullptr);
}

void Response::ArenaDtor(void* object) {
  Response* _this = reinterpret_cast< Response* >(object);
  (void)_this;
}
void Response::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void Response::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}

void Response::Clear() {
// @@protoc_insertion_point(message_clear_start:ua_blackjack.Response)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  args_.Clear();
  cached_has_bits = _has_bits_[0];
  if (cached_has_bits & 0x00000007u) {
    ::memset(&status_, 0, static_cast<size_t>(
        reinterpret_cast<char*>(&stamp_) -
        reinterpret_cast<char*>(&status_)) + sizeof(stamp_));
  }
  _has_bits_.Clear();
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* Response::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  _Internal::HasBits has_bits{};
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    CHK_(ptr);
    switch (tag >> 3) {
      // int64 status = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 8)) {
          _Internal::set_has_status(&has_bits);
          status_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // int64 uid = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 16)) {
          _Internal::set_has_uid(&has_bits);
          uid_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // int64 stamp = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 24)) {
          _Internal::set_has_stamp(&has_bits);
          stamp_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // repeated string args = 4;
      case 4:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 34)) {
          ptr -= 1;
          do {
            ptr += 1;
            auto str = _internal_add_args();
            ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParser(str, ptr, ctx);
            CHK_(::PROTOBUF_NAMESPACE_ID::internal::VerifyUTF8(str, "ua_blackjack.Response.args"));
            CHK_(ptr);
            if (!ctx->DataAvailable(ptr)) break;
          } while (::PROTOBUF_NAMESPACE_ID::internal::ExpectTag<34>(ptr));
        } else goto handle_unusual;
        continue;
      default: {
      handle_unusual:
        if ((tag & 7) == 4 || tag == 0) {
          ctx->SetLastTag(tag);
          goto success;
        }
        ptr = UnknownFieldParse(tag,
            _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
            ptr, ctx);
        CHK_(ptr != nullptr);
        continue;
      }
    }  // switch
  }  // while
success:
  _has_bits_.Or(has_bits);
  return ptr;
failure:
  ptr = nullptr;
  goto success;
#undef CHK_
}

::PROTOBUF_NAMESPACE_ID::uint8* Response::_InternalSerialize(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:ua_blackjack.Response)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // int64 status = 1;
  if (_internal_has_status()) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt64ToArray(1, this->_internal_status(), target);
  }

  // int64 uid = 2;
  if (_internal_has_uid()) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt64ToArray(2, this->_internal_uid(), target);
  }

  // int64 stamp = 3;
  if (_internal_has_stamp()) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt64ToArray(3, this->_internal_stamp(), target);
  }

  // repeated string args = 4;
  for (int i = 0, n = this->_internal_args_size(); i < n; i++) {
    const auto& s = this->_internal_args(i);
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      s.data(), static_cast<int>(s.length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "ua_blackjack.Response.args");
    target = stream->WriteString(4, s, target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:ua_blackjack.Response)
  return target;
}

size_t Response::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:ua_blackjack.Response)
  size_t total_size = 0;

  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // repeated string args = 4;
  total_size += 1 *
      ::PROTOBUF_NAMESPACE_ID::internal::FromIntSize(args_.size());
  for (int i = 0, n = args_.size(); i < n; i++) {
    total_size += ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
      args_.Get(i));
  }

  cached_has_bits = _has_bits_[0];
  if (cached_has_bits & 0x00000007u) {
    // int64 status = 1;
    if (cached_has_bits & 0x00000001u) {
      total_size += 1 +
        ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int64Size(
          this->_internal_status());
    }

    // int64 uid = 2;
    if (cached_has_bits & 0x00000002u) {
      total_size += 1 +
        ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int64Size(
          this->_internal_uid());
    }

    // int64 stamp = 3;
    if (cached_has_bits & 0x00000004u) {
      total_size += 1 +
        ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int64Size(
          this->_internal_stamp());
    }

  }
  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void Response::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:ua_blackjack.Response)
  GOOGLE_DCHECK_NE(&from, this);
  const Response* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<Response>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:ua_blackjack.Response)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:ua_blackjack.Response)
    MergeFrom(*source);
  }
}

void Response::MergeFrom(const Response& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:ua_blackjack.Response)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  args_.MergeFrom(from.args_);
  cached_has_bits = from._has_bits_[0];
  if (cached_has_bits & 0x00000007u) {
    if (cached_has_bits & 0x00000001u) {
      status_ = from.status_;
    }
    if (cached_has_bits & 0x00000002u) {
      uid_ = from.uid_;
    }
    if (cached_has_bits & 0x00000004u) {
      stamp_ = from.stamp_;
    }
    _has_bits_[0] |= cached_has_bits;
  }
}

void Response::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:ua_blackjack.Response)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Response::CopyFrom(const Response& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:ua_blackjack.Response)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Response::IsInitialized() const {
  return true;
}

void Response::InternalSwap(Response* other) {
  using std::swap;
  _internal_metadata_.Swap<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(&other->_internal_metadata_);
  swap(_has_bits_[0], other->_has_bits_[0]);
  args_.InternalSwap(&other->args_);
  ::PROTOBUF_NAMESPACE_ID::internal::memswap<
      PROTOBUF_FIELD_OFFSET(Response, stamp_)
      + sizeof(Response::stamp_)
      - PROTOBUF_FIELD_OFFSET(Response, status_)>(
          reinterpret_cast<char*>(&status_),
          reinterpret_cast<char*>(&other->status_));
}

::PROTOBUF_NAMESPACE_ID::Metadata Response::GetMetadata() const {
  return GetMetadataStatic();
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace ua_blackjack
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::ua_blackjack::Request* Arena::CreateMaybeMessage< ::ua_blackjack::Request >(Arena* arena) {
  return Arena::CreateMessageInternal< ::ua_blackjack::Request >(arena);
}
template<> PROTOBUF_NOINLINE ::ua_blackjack::Response* Arena::CreateMaybeMessage< ::ua_blackjack::Response >(Arena* arena) {
  return Arena::CreateMessageInternal< ::ua_blackjack::Response >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
