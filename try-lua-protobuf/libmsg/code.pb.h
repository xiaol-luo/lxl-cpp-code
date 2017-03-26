// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: code.proto

#ifndef PROTOBUF_code_2eproto__INCLUDED
#define PROTOBUF_code_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3001000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3001000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
class Code;
class CodeDefaultTypeInternal;
extern CodeDefaultTypeInternal _Code_default_instance_;

// Internal implementation detail -- do not call these.
void protobuf_AddDesc_code_2eproto();
void protobuf_InitDefaults_code_2eproto();

// ===================================================================

class Code : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:Code) */ {
 public:
  Code();
  virtual ~Code();

  Code(const Code& from);

  inline Code& operator=(const Code& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const Code& default_instance();

  static inline const Code* internal_default_instance() {
    return reinterpret_cast<const Code*>(
               &_Code_default_instance_);
  }

  void Swap(Code* other);

  // implements Message ----------------------------------------------

  inline Code* New() const PROTOBUF_FINAL { return New(NULL); }

  Code* New(::google::protobuf::Arena* arena) const PROTOBUF_FINAL;
  void CopyFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void MergeFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void CopyFrom(const Code& from);
  void MergeFrom(const Code& from);
  void Clear() PROTOBUF_FINAL;
  bool IsInitialized() const PROTOBUF_FINAL;

  size_t ByteSizeLong() const PROTOBUF_FINAL;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) PROTOBUF_FINAL;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const PROTOBUF_FINAL;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const PROTOBUF_FINAL;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output)
      const PROTOBUF_FINAL {
    return InternalSerializeWithCachedSizesToArray(false, output);
  }
  int GetCachedSize() const PROTOBUF_FINAL { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const PROTOBUF_FINAL;
  void InternalSwap(Code* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const PROTOBUF_FINAL;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // int32 msgID = 1;
  void clear_msgid();
  static const int kMsgIDFieldNumber = 1;
  ::google::protobuf::int32 msgid() const;
  void set_msgid(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:Code)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::int32 msgid_;
  mutable int _cached_size_;
  friend void  protobuf_InitDefaults_code_2eproto_impl();
  friend void  protobuf_AddDesc_code_2eproto_impl();
  friend const ::google::protobuf::uint32* protobuf_Offsets_code_2eproto();
  friend void protobuf_ShutdownFile_code_2eproto();

};
// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
// Code

// int32 msgID = 1;
inline void Code::clear_msgid() {
  msgid_ = 0;
}
inline ::google::protobuf::int32 Code::msgid() const {
  // @@protoc_insertion_point(field_get:Code.msgID)
  return msgid_;
}
inline void Code::set_msgid(::google::protobuf::int32 value) {
  
  msgid_ = value;
  // @@protoc_insertion_point(field_set:Code.msgID)
}

#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS

// @@protoc_insertion_point(namespace_scope)


// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_code_2eproto__INCLUDED