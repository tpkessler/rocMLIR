// Test without serialization:
// RUN: %clang_cc1 -std=c++17 -triple x86_64-unknown-unknown -ast-dump %s \
// RUN: | FileCheck -strict-whitespace %s
//
// Test with serialization:
// RUN: %clang_cc1 -std=c++17 -triple x86_64-unknown-unknown -emit-pch -o %t %s
// RUN: %clang_cc1 -x c++ -std=c++17 -triple x86_64-unknown-unknown -include-pch %t -ast-dump-all /dev/null \
// RUN: | sed -e "s/ <undeserialized declarations>//" -e "s/ imported//" \
// RUN: | FileCheck --strict-whitespace %s

template <typename Ty>
// CHECK: FunctionTemplateDecl 0x{{[^ ]*}} <{{.*}}:1, line:[[@LINE+2]]:10> col:6 a
// CHECK-NEXT: TemplateTypeParmDecl 0x{{[^ ]*}} <line:[[@LINE-2]]:11, col:20> col:20 referenced typename depth 0 index 0 Ty
void a(Ty);

template <typename... Ty>
// CHECK: FunctionTemplateDecl 0x{{[^ ]*}} <line:[[@LINE-1]]:1, line:[[@LINE+2]]:13> col:6 b
// CHECK-NEXT: TemplateTypeParmDecl 0x{{[^ ]*}} <line:[[@LINE-2]]:11, col:23> col:23 referenced typename depth 0 index 0 ... Ty
void b(Ty...);

template <typename Ty, typename Uy>
// CHECK: FunctionTemplateDecl 0x{{[^ ]*}} <line:[[@LINE-1]]:1, line:[[@LINE+3]]:14> col:6 c
// CHECK-NEXT: TemplateTypeParmDecl 0x{{[^ ]*}} <line:[[@LINE-2]]:11, col:20> col:20 referenced typename depth 0 index 0 Ty
// CHECK-NEXT: TemplateTypeParmDecl 0x{{[^ ]*}} <col:24, col:33> col:33 referenced typename depth 0 index 1 Uy
void c(Ty, Uy);

template <>
void c<float, int>(float, int);
// CHECK: FunctionDecl 0x{{[^ ]*}} prev 0x{{[^ ]*}} <line:[[@LINE-2]]:1, line:[[@LINE-1]]:30> col:6 c 'void (float, int)'
// CHECK: TemplateArgument type 'float'
// CHECK: TemplateArgument type 'int'

template <typename Ty, template<typename> typename Uy>
// CHECK: FunctionTemplateDecl 0x{{[^ ]*}} <line:[[@LINE-1]]:1, line:[[@LINE+4]]:18> col:6 d
// CHECK-NEXT: TemplateTypeParmDecl 0x{{[^ ]*}} <line:[[@LINE-2]]:11, col:20> col:20 referenced typename depth 0 index 0 Ty
// CHECK-NEXT: TemplateTemplateParmDecl 0x{{[^ ]*}} <col:24, col:52> col:52 depth 0 index 1 Uy
// CHECK-NEXT: TemplateTypeParmDecl 0x{{[^ ]*}} <col:33> col:41 typename depth 1 index 0
void d(Ty, Uy<Ty>);

template <class Ty>
// CHECK: FunctionTemplateDecl 0x{{[^ ]*}} <line:[[@LINE-1]]:1, line:[[@LINE+2]]:10> col:6 e
// CHECK-NEXT: TemplateTypeParmDecl 0x{{[^ ]*}} <line:[[@LINE-2]]:11, col:17> col:17 referenced class depth 0 index 0 Ty
void e(Ty);

template <int N>
// CHECK: FunctionTemplateDecl 0x{{[^ ]*}} <line:[[@LINE-1]]:1, line:[[@LINE+2]]:17> col:6 f
// CHECK-NEXT: NonTypeTemplateParmDecl 0x{{[^ ]*}} <line:[[@LINE-2]]:11, col:15> col:15 referenced 'int' depth 0 index 0 N
void f(int i = N);

template <typename Ty = int>
// CHECK: FunctionTemplateDecl 0x{{[^ ]*}} <line:[[@LINE-1]]:1, line:[[@LINE+3]]:10> col:6 g
// CHECK-NEXT: TemplateTypeParmDecl 0x{{[^ ]*}} <line:[[@LINE-2]]:11, col:25> col:20 referenced typename depth 0 index 0 Ty
// CHECK-NEXT: TemplateArgument type 'int'
void g(Ty);

template <typename = void>
// CHECK: FunctionTemplateDecl 0x{{[^ ]*}} <line:[[@LINE-1]]:1, line:[[@LINE+3]]:8> col:6 h
// CHECK-NEXT: TemplateTypeParmDecl 0x{{[^ ]*}} <line:[[@LINE-2]]:11, col:22> col:20 typename depth 0 index 0
// CHECK-NEXT: TemplateArgument type 'void'
void h();

template <typename Ty>
// CHECK: ClassTemplateDecl 0x{{[^ ]*}} <line:[[@LINE-1]]:1, line:[[@LINE+3]]:11> col:8 R
// CHECK-NEXT: TemplateTypeParmDecl 0x{{[^ ]*}} <line:[[@LINE-2]]:11, col:20> col:20 typename depth 0 index 0 Ty
// CHECK: ClassTemplateSpecialization 0x{{[^ ]*}} 'R'
struct R {};

template <>
// CHECK: ClassTemplateSpecializationDecl 0x{{[^ ]*}} <line:[[@LINE-1]]:1, line:[[@LINE+2]]:16> col:8 struct R definition
// CHECK: TemplateArgument type 'int'
struct R<int> {};

template <typename Ty, class Uy>
// CHECK: ClassTemplateDecl 0x{{[^ ]*}} <line:[[@LINE-1]]:1, line:[[@LINE+3]]:11> col:8 S
// CHECK-NEXT: TemplateTypeParmDecl 0x{{[^ ]*}} <line:[[@LINE-2]]:11, col:20> col:20 typename depth 0 index 0 Ty
// CHECK-NEXT: TemplateTypeParmDecl 0x{{[^ ]*}} <col:24, col:30> col:30 class depth 0 index 1 Uy
struct S {};

template <typename Ty>
// CHECK: ClassTemplatePartialSpecializationDecl 0x{{[^ ]*}} <line:[[@LINE-1]]:1, line:[[@LINE+4]]:20> col:8 struct S definition
// CHECK: TemplateArgument type 'type-parameter-0-0'
// CHECK: TemplateArgument type 'int'
// CHECK: TemplateTypeParmDecl 0x{{[^ ]*}} <line:[[@LINE-4]]:11, col:20> col:20 referenced typename depth 0 index 0 Ty
struct S<Ty, int> {};

template <auto>
// CHECK: ClassTemplateDecl 0x{{[^ ]*}} <line:[[@LINE-1]]:1, line:[[@LINE+2]]:11> col:8 T
// CHECK-NEXT: NonTypeTemplateParmDecl 0x{{[^ ]*}} <line:[[@LINE-2]]:11> col:15 'auto' depth 0 index 0
struct T {};

template <decltype(auto)>
// CHECK: ClassTemplateDecl 0x{{[^ ]*}} <line:[[@LINE-1]]:1, line:[[@LINE+2]]:11> col:8 U
// CHECK-NEXT: NonTypeTemplateParmDecl 0x{{[^ ]*}} <line:[[@LINE-2]]:11, col:24> col:25 'decltype(auto)' depth 0 index 0
struct U {};

template <typename Ty>
// CHECK: ClassTemplateDecl 0x{{[^ ]*}} <line:[[@LINE-1]]:1, line:[[@LINE+7]]:1> line:[[@LINE+2]]:8 V
// CHECK-NEXT: TemplateTypeParmDecl 0x{{[^ ]*}} <line:[[@LINE-2]]:11, col:20> col:20 typename depth 0 index 0 Ty
struct V {
  template <typename Uy>
  // CHECK: FunctionTemplateDecl 0x{{[^ ]*}} <line:[[@LINE-1]]:3, line:[[@LINE+2]]:10> col:8 f
  // CHECK-NEXT: TemplateTypeParmDecl 0x{{[^ ]*}} <line:[[@LINE-2]]:13, col:22> col:22 typename depth 1 index 0 Uy
  void f();
};

template <typename Ty>
template <typename Uy>
// CHECK: FunctionTemplateDecl 0x{{[^ ]*}} parent 0x{{[^ ]*}} prev 0x{{[^ ]*}} <line:[[@LINE-1]]:1, line:[[@LINE+2]]:18> col:13 f
// CHECK-NEXT: TemplateTypeParmDecl 0x{{[^ ]*}} <line:[[@LINE-2]]:11, col:20> col:20 typename depth 1 index 0 Uy
void V<Ty>::f() {}

namespace PR55886 {
template <class T> struct C {
  template <class U> using type1 = U(T);
};
using type2 = typename C<int>::type1<void>;
// CHECK:      TypeAliasDecl 0x{{[^ ]*}} <line:[[@LINE-1]]:1, col:42> col:7 type2 'typename C<int>::type1<void>':'void (int)'
// CHECK-NEXT: ElaboratedType 0x{{[^ ]*}} 'typename C<int>::type1<void>' sugar
// CHECK-NEXT: TemplateSpecializationType 0x{{[^ ]*}} 'type1<void>' sugar alias type1
// CHECK-NEXT: TemplateArgument type 'void'
// CHECK-NEXT: BuiltinType 0x{{[^ ]*}} 'void'
// CHECK-NEXT: FunctionProtoType 0x{{[^ ]*}} 'void (int)' cdecl
// CHECK-NEXT: SubstTemplateTypeParmType 0x{{[^ ]*}} 'void' sugar
// CHECK-NEXT: TemplateTypeParmType 0x{{[^ ]*}} 'U' dependent depth 0 index 0
// CHECK-NEXT: TemplateTypeParm 0x{{[^ ]*}} 'U'
// CHECK-NEXT: BuiltinType 0x{{[^ ]*}} 'void'
// CHECK-NEXT: SubstTemplateTypeParmType 0x{{[^ ]*}} 'int' sugar
// CHECK-NEXT: TemplateTypeParmType 0x{{[^ ]*}} 'T' dependent depth 0 index 0
} // namespace PR55886

namespace PR56099 {
template <typename... As> struct Y;
template <typename... Bs> using Z = Y<Bs...>;
template <typename... Cs> struct foo {
  template <typename... Ds> using bind = Z<Ds..., Cs...>;
};
using t1 = foo<int, short>::bind<char, float>;
// CHECK:      TemplateSpecializationType 0x{{[^ ]*}} 'Y<char, float, int, short>' sugar Y
// CHECK:      SubstTemplateTypeParmType 0x{{[^ ]*}} 'char' sugar pack_index 3
// CHECK-NEXT: TemplateTypeParmType 0x{{[^ ]*}} 'Bs' dependent contains_unexpanded_pack depth 0 index 0 pack
// CHECK:      SubstTemplateTypeParmType 0x{{[^ ]*}} 'float' sugar pack_index 2
// CHECK-NEXT: TemplateTypeParmType 0x{{[^ ]*}} 'Bs' dependent contains_unexpanded_pack depth 0 index 0 pack
// CHECK:      SubstTemplateTypeParmType 0x{{[^ ]*}} 'int' sugar pack_index 1
// CHECK-NEXT: TemplateTypeParmType 0x{{[^ ]*}} 'Bs' dependent contains_unexpanded_pack depth 0 index 0 pack
// CHECK:      SubstTemplateTypeParmType 0x{{[^ ]*}} 'short' sugar pack_index 0
// CHECK-NEXT: TemplateTypeParmType 0x{{[^ ]*}} 'Bs' dependent contains_unexpanded_pack depth 0 index 0 pack

template <typename... T> struct D {
  template <typename... U> using B = int(int (*...p)(T, U));
};
using t2 = D<float, char>::B<int, short>;
// CHECK:      TemplateSpecializationType 0x{{[^ ]*}} 'B<int, short>' sugar alias B
// CHECK:      FunctionProtoType 0x{{[^ ]*}} 'int (int (*)(float, int), int (*)(char, short))' cdecl
// CHECK:      FunctionProtoType 0x{{[^ ]*}} 'int (float, int)' cdecl
// CHECK:      SubstTemplateTypeParmType 0x{{[^ ]*}} 'float' sugar pack_index 1
// CHECK-NEXT: TemplateTypeParmType 0x{{[^ ]*}} 'T' dependent contains_unexpanded_pack depth 0 index 0 pack
// CHECK:      SubstTemplateTypeParmType 0x{{[^ ]*}} 'int' sugar pack_index 1
// CHECK-NEXT: TemplateTypeParmType 0x{{[^ ]*}} 'U' dependent contains_unexpanded_pack depth 0 index 0 pack
// CHECK:      FunctionProtoType 0x{{[^ ]*}} 'int (char, short)' cdecl
// CHECK:      SubstTemplateTypeParmType 0x{{[^ ]*}} 'char' sugar pack_index 0
// CHECK-NEXT: TemplateTypeParmType 0x{{[^ ]*}} 'T' dependent contains_unexpanded_pack depth 0 index 0 pack
// CHECK:      SubstTemplateTypeParmType 0x{{[^ ]*}} 'short' sugar pack_index 0
// CHECK-NEXT: TemplateTypeParmType 0x{{[^ ]*}} 'U' dependent contains_unexpanded_pack depth 0 index 0 pack
} // namespace PR56099
