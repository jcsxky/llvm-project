// NOTE: Assertions have been autogenerated by utils/update_cc_test_checks.py

// RUN: %clang_cc1 -no-opaque-pointers -triple x86_64 -S -emit-llvm -o - %s | FileCheck %s
//
// Verifies that clang detects memcpy inline version and uses it instead of the builtin.

typedef unsigned long size_t;

// Clang requires these attributes for a function to be redefined.
#define AVAILABLE_EXTERNALLY extern inline __attribute__((always_inline)) __attribute__((gnu_inline))

// Clang recognizes an inline builtin and renames it to prevent conflict with builtins.
AVAILABLE_EXTERNALLY void *memcpy(void *a, const void *b, size_t c) {
  asm("# memcpy.inline marker");
  return __builtin_memcpy(a, b, c);
}

// CHECK-LABEL: @foo(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[A_ADDR_I:%.*]] = alloca i8*, align 8
// CHECK-NEXT:    [[B_ADDR_I:%.*]] = alloca i8*, align 8
// CHECK-NEXT:    [[C_ADDR_I:%.*]] = alloca i64, align 8
// CHECK-NEXT:    [[A_ADDR:%.*]] = alloca i8*, align 8
// CHECK-NEXT:    [[B_ADDR:%.*]] = alloca i8*, align 8
// CHECK-NEXT:    [[C_ADDR:%.*]] = alloca i64, align 8
// CHECK-NEXT:    store i8* [[A:%.*]], i8** [[A_ADDR]], align 8
// CHECK-NEXT:    store i8* [[B:%.*]], i8** [[B_ADDR]], align 8
// CHECK-NEXT:    store i64 [[C:%.*]], i64* [[C_ADDR]], align 8
// CHECK-NEXT:    [[TMP0:%.*]] = load i8*, i8** [[A_ADDR]], align 8
// CHECK-NEXT:    [[TMP1:%.*]] = load i8*, i8** [[B_ADDR]], align 8
// CHECK-NEXT:    [[TMP2:%.*]] = load i64, i64* [[C_ADDR]], align 8
// CHECK-NEXT:    store i8* [[TMP0]], i8** [[A_ADDR_I]], align 8
// CHECK-NEXT:    store i8* [[TMP1]], i8** [[B_ADDR_I]], align 8
// CHECK-NEXT:    store i64 [[TMP2]], i64* [[C_ADDR_I]], align 8
// CHECK-NEXT:    call void asm sideeffect "# memcpy.inline marker", "~{dirflag},~{fpsr},~{flags}"() #[[ATTR3:[0-9]+]], !srcloc !2
// CHECK-NEXT:    [[TMP3:%.*]] = load i8*, i8** [[A_ADDR_I]], align 8
// CHECK-NEXT:    [[TMP4:%.*]] = load i8*, i8** [[B_ADDR_I]], align 8
// CHECK-NEXT:    [[TMP5:%.*]] = load i64, i64* [[C_ADDR_I]], align 8
// CHECK-NEXT:    call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 [[TMP3]], i8* align 1 [[TMP4]], i64 [[TMP5]], i1 false) #[[ATTR3]]
// CHECK-NEXT:    ret i8* [[TMP3]]
//
void *foo(void *a, const void *b, size_t c) {
  return memcpy(a, b, c);
}

// CHECK-LABEL: @bar(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[A_ADDR:%.*]] = alloca i8*, align 8
// CHECK-NEXT:    [[B_ADDR:%.*]] = alloca i8*, align 8
// CHECK-NEXT:    [[C_ADDR:%.*]] = alloca i64, align 8
// CHECK-NEXT:    [[CPY:%.*]] = alloca i8* (i8*, i8*, i64)*, align 8
// CHECK-NEXT:    store i8* [[A:%.*]], i8** [[A_ADDR]], align 8
// CHECK-NEXT:    store i8* [[B:%.*]], i8** [[B_ADDR]], align 8
// CHECK-NEXT:    store i64 [[C:%.*]], i64* [[C_ADDR]], align 8
// CHECK-NEXT:    [[TMP0:%.*]] = load i64, i64* [[C_ADDR]], align 8
// CHECK-NEXT:    [[CMP:%.*]] = icmp ugt i64 [[TMP0]], 10
// CHECK-NEXT:    [[TMP1:%.*]] = zext i1 [[CMP]] to i64
// CHECK-NEXT:    [[COND:%.*]] = select i1 [[CMP]], i8* (i8*, i8*, i64)* @memcpy, i8* (i8*, i8*, i64)* @foo
// CHECK-NEXT:    store i8* (i8*, i8*, i64)* [[COND]], i8* (i8*, i8*, i64)** [[CPY]], align 8
// CHECK-NEXT:    [[TMP2:%.*]] = load i8* (i8*, i8*, i64)*, i8* (i8*, i8*, i64)** [[CPY]], align 8
// CHECK-NEXT:    [[TMP3:%.*]] = load i8*, i8** [[A_ADDR]], align 8
// CHECK-NEXT:    [[TMP4:%.*]] = load i8*, i8** [[B_ADDR]], align 8
// CHECK-NEXT:    [[TMP5:%.*]] = load i64, i64* [[C_ADDR]], align 8
// CHECK-NEXT:    [[CALL:%.*]] = call i8* [[TMP2]](i8* noundef [[TMP3]], i8* noundef [[TMP4]], i64 noundef [[TMP5]])
// CHECK-NEXT:    ret void
//
void bar(void *a, const void *b, size_t c) {
  void *(*cpy)(void *, const void *, size_t) = c > 10 ? memcpy : foo;
  cpy(a, b, c);
}
