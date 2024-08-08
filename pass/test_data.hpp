#ifndef CANONICALIZATION_TEST_DATA
#define CANONICALIZATION_TEST_DATA

#include <iostream>

const std::string test_ir1 = R"//(
@g = global i32 123, align 4
@gp = global i32* @g, align 8
@vec = global [3 x i32] [i32 1, i32 2, i32 3], align 4
@intptr = common global i32* null, align 8

define i32 @f0(i32 %a) {
  %1 = load i32*, i32** @gp, align 8
  %2 = load i32, i32* %1, align 4
  %3 = add nsw i32 %2, %a
  ret i32 %3
}

define i32 @f1(i32) {
  %2 = load i32, i32* getelementptr inbounds ([3 x i32], [3 x i32]* @vec, i64 0, i64 0), align 4
  %3 = add nsw i32 %2, %0
  ret i32 %3
}
)//";

const std::string test_ir2 = R"//(
source_filename = "test"
target datalayout = "e-m:e-p:64:64-i64:64-f80:128-n8:16:32:64-S128"

@global_var_600ff8 = local_unnamed_addr global i64 0
@global_var_600e20 = local_unnamed_addr global i64 0
@global_var_400944 = constant [9 x i8] c"%d\0A%d\0A%d\00"
@global_var_600e10 = local_unnamed_addr global i64 4195648
@global_var_600e18 = local_unnamed_addr global i64 4195616
@global_var_601040 = local_unnamed_addr global i8 0

define i64 @f1(i32 %arg1) local_unnamed_addr {
dec_label_pc_400570:
  %0 = add i32 %arg1, 32, !insn.addr !0
  %1 = zext i32 %0 to i64, !insn.addr !1
  ret i64 %1, !insn.addr !2
}

define i64 @f2(i32 %arg1) local_unnamed_addr {
dec_label_pc_400590:
  %0 = add i32 %arg1, 32, !insn.addr !3
  %1 = zext i32 %0 to i64, !insn.addr !4
  ret i64 %1, !insn.addr !5
}

define i64 @f3(i32 %arg1) local_unnamed_addr {
dec_label_pc_4005c0:
  %0 = add i32 %arg1, 32, !insn.addr !6
  %1 = zext i32 %0 to i64, !insn.addr !7
  ret i64 %1, !insn.addr !8
}

define i64 @f4(i32 %arg1) local_unnamed_addr {
dec_label_pc_400600:
  %storemerge = add i32 %arg1, 32
  %0 = zext i32 %storemerge to i64, !insn.addr !9
  ret i64 %0, !insn.addr !10
}

define i64 @f5(i32 %arg1) local_unnamed_addr {
dec_label_pc_400660:
  %stack_var_-12.0 = add i32 %arg1, 32
  %0 = zext i32 %stack_var_-12.0 to i64, !insn.addr !11
  ret i64 %0, !insn.addr !12
}

define i64 @main(i64 %argc, i8** %argv) local_unnamed_addr {
dec_label_pc_400840:
  %0 = ptrtoint i8** %argv to i64
  %1 = add i64 %0, 8, !insn.addr !13
  %2 = inttoptr i64 %1 to i64*, !insn.addr !13
  %3 = load i64, i64* %2, align 8, !insn.addr !13
  %4 = inttoptr i64 %3 to i8*, !insn.addr !14
  %5 = call i32 @atoi(i8* %4), !insn.addr !14
  %6 = call i64 @f1(i32 %5), !insn.addr !15
  %7 = call i64 @f2(i32 %5), !insn.addr !16
  %8 = call i64 @f3(i32 %5), !insn.addr !17
  %9 = and i64 %6, 4294967295, !insn.addr !18
  %10 = and i64 %7, 4294967295, !insn.addr !19
  %11 = and i64 %8, 4294967295, !insn.addr !20
  %12 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @global_var_400944, i64 0, i64 0), i64 %9, i64 %10, i64 %11), !insn.addr !21
  %13 = call i64 @f4(i32 xor (i32 ptrtoint ([9 x i8]* @global_var_400944 to i32), i32 ptrtoint ([9 x i8]* @global_var_400944 to i32))), !insn.addr !22
  %14 = call i64 @f5(i32 xor (i32 xor (i32 ptrtoint ([9 x i8]* @global_var_400944 to i32), i32 ptrtoint ([9 x i8]* @global_var_400944 to i32)), i32 xor (i32 ptrtoint ([9 x i8]* @global_var_400944 to i32), i32 ptrtoint ([9 x i8]* @global_var_400944 to i32)))), !insn.addr !23
  ret i64 0, !insn.addr !24
}

declare i32 @printf(i8*, ...) local_unnamed_addr

declare i32 @atoi(i8*) local_unnamed_addr

!0 = !{i64 4195706}
!1 = !{i64 4195712}
!2 = !{i64 4195716}
!3 = !{i64 4195754}
!4 = !{i64 4195760}
!5 = !{i64 4195764}
!6 = !{i64 4195814}
!7 = !{i64 4195826}
!8 = !{i64 4195830}
!9 = !{i64 4195916}
!10 = !{i64 4195920}
!11 = !{i64 4196159}
!12 = !{i64 4196163}
!13 = !{i64 4196442}
!14 = !{i64 4196448}
!15 = !{i64 4196459}
!16 = !{i64 4196470}
!17 = !{i64 4196481}
!18 = !{i64 4196496}
!19 = !{i64 4196499}
!20 = !{i64 4196502}
!21 = !{i64 4196506}
!22 = !{i64 4196516}
!23 = !{i64 4196526}
!24 = !{i64 4196543}
)//";

const std::string test_ir3 = R"//(
%type1 =  type {
    i32,
    i32,
    double
}
%type2 = type {
    i32,
    i32,
    %type1
}

define i32* @f0(i32 %a, %type2* %b) {
  %1 = getelementptr %type2, %type2* %b, i32 0, i32 1
  ret i32* %1
}
)//";

const std::string test_ir4 = R"//(
; ModuleID = 'function_struct_test.bc'
source_filename = "function_struct_test.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.Person = type { [50 x i8], i32, %struct.Address }
%struct.Address = type { [50 x i8], [50 x i8], [50 x i8], [10 x i8] }

@.str = private unnamed_addr constant [10 x i8] c"Name: %s\0A\00", align 1
@.str.1 = private unnamed_addr constant [9 x i8] c"Age: %d\0A\00", align 1
@.str.3 = private unnamed_addr constant [12 x i8] c"Street: %s\0A\00", align 1
@.str.4 = private unnamed_addr constant [10 x i8] c"City: %s\0A\00", align 1
@.str.5 = private unnamed_addr constant [11 x i8] c"State: %s\0A\00", align 1
@.str.6 = private unnamed_addr constant [9 x i8] c"Zip: %s\0A\00", align 1
@__const.main.person1 = private unnamed_addr constant %struct.Person { [50 x i8] c"John Doe\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00", i32 30, %struct.Address { [50 x i8] c"123 Main St\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00", [50 x i8] c"Exampleville\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00", [50 x i8] c"CA\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00", [10 x i8] c"12345\00\00\00\00\00" } }, align 4
@.str.7 = private unnamed_addr constant [16 x i8] c"Enter new age: \00", align 1
@.str.8 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@str = private unnamed_addr constant [9 x i8] c"Address:\00", align 1

; Function Attrs: noinline nounwind uwtable
define dso_local void @printPerson(%struct.Person* byval align 8) local_unnamed_addr #0 {
  %2 = getelementptr inbounds %struct.Person, %struct.Person* %0, i64 0, i32 0, i64 0
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str, i64 0, i64 0), i8* nonnull %2)
  %4 = getelementptr inbounds %struct.Person, %struct.Person* %0, i64 0, i32 1
  %5 = load i32, i32* %4, align 4, !tbaa !2
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str.1, i64 0, i64 0), i32 %5)
  %7 = call i32 @puts(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str, i64 0, i64 0))
  %8 = getelementptr inbounds %struct.Person, %struct.Person* %0, i64 0, i32 2, i32 0, i64 0
  %9 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @.str.3, i64 0, i64 0), i8* nonnull %8)
  %10 = getelementptr inbounds %struct.Person, %struct.Person* %0, i64 0, i32 2, i32 1, i64 0
  %11 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.4, i64 0, i64 0), i8* nonnull %10)
  %12 = getelementptr inbounds %struct.Person, %struct.Person* %0, i64 0, i32 2, i32 2, i64 0
  %13 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @.str.5, i64 0, i64 0), i8* nonnull %12)
  %14 = getelementptr inbounds %struct.Person, %struct.Person* %0, i64 0, i32 2, i32 3, i64 0
  %15 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str.6, i64 0, i64 0), i8* nonnull %14)
  ret void
}

; Function Attrs: nounwind
declare dso_local i32 @printf(i8* nocapture readonly, ...) local_unnamed_addr #1

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
  %1 = alloca %struct.Person, align 8
  %2 = getelementptr inbounds %struct.Person, %struct.Person* %1, i64 0, i32 0, i64 0
  call void @llvm.lifetime.start.p0i8(i64 216, i8* nonnull %2) #3
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* nonnull align 8 %2, i8* align 4 getelementptr inbounds (%struct.Person, %struct.Person* @__const.main.person1, i64 0, i32 0, i64 0), i64 216, i1 false)
  %3 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @.str.7, i64 0, i64 0))
  %4 = getelementptr inbounds %struct.Person, %struct.Person* %1, i64 0, i32 1
  %5 = call i32 (i8*, ...) @__isoc99_scanf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.8, i64 0, i64 0), i32* nonnull %4)
  call void @printPerson(%struct.Person* byval nonnull align 8 %1)
  call void @llvm.lifetime.end.p0i8(i64 216, i8* nonnull %2) #3
  ret i32 0
}

; Function Attrs: argmemonly nounwind
declare void @llvm.lifetime.start.p0i8(i64, i8* nocapture) #2

; Function Attrs: argmemonly nounwind
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* nocapture writeonly, i8* nocapture readonly, i64, i1) #2

; Function Attrs: nounwind
declare dso_local i32 @__isoc99_scanf(i8* nocapture readonly, ...) local_unnamed_addr #1

; Function Attrs: argmemonly nounwind
declare void @llvm.lifetime.end.p0i8(i64, i8* nocapture) #2

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @main2() local_unnamed_addr #0 {
  %1 = tail call i32 @main()
  ret i32 0
}

; Function Attrs: nounwind
declare i32 @puts(i8* nocapture readonly) local_unnamed_addr #3

attributes #0 = { noinline nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { argmemonly nounwind }
attributes #3 = { nounwind }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 8.0.1 (tags/RELEASE_801/final)"}
!2 = !{!3, !6, i64 52}
!3 = !{!"Person", !4, i64 0, !6, i64 52, !7, i64 56}
!4 = !{!"omnipotent char", !5, i64 0}
!5 = !{!"Simple C/C++ TBAA"}
!6 = !{!"int", !4, i64 0}
!7 = !{!"Address", !4, i64 0, !4, i64 50, !4, i64 100, !4, i64 150}
)//";

const std::string test_ir5 = R"//(
; ModuleID = 'global_struct.bc'
source_filename = "test_C_files/global_struct.cpp"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%"class.std::ios_base::Init" = type { i8 }
%struct.OuterStruct = type { %struct.InnerStruct }
%struct.InnerStruct = type { i32 }

@_ZStL8__ioinit = internal global %"class.std::ios_base::Init" zeroinitializer, align 1
@__dso_handle = external hidden global i8
@globalB = dso_local global %struct.OuterStruct { %struct.InnerStruct { i32 10 } }, align 4
@globalA = dso_local local_unnamed_addr global %struct.OuterStruct* @globalB, align 8
@.str = private unnamed_addr constant [23 x i8] c"Global Variable A: %p\0A\00", align 1
@llvm.global_ctors = appending global [1 x { i32, void ()*, i8* }] [{ i32, void ()*, i8* } { i32 65535, void ()* @_GLOBAL__sub_I_global_struct.cpp, i8* null }]

declare dso_local void @_ZNSt8ios_base4InitC1Ev(%"class.std::ios_base::Init"*) unnamed_addr #0

; Function Attrs: nounwind
declare dso_local void @_ZNSt8ios_base4InitD1Ev(%"class.std::ios_base::Init"*) unnamed_addr #1

; Function Attrs: nounwind
declare dso_local i32 @__cxa_atexit(void (i8*)*, i8*, i8*) local_unnamed_addr #2

; Function Attrs: noinline norecurse nounwind uwtable
define dso_local i32 @main() local_unnamed_addr #3 {
  %1 = load %struct.OuterStruct*, %struct.OuterStruct** @globalA, align 8, !tbaa !2
  %2 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @.str, i64 0, i64 0), %struct.OuterStruct* %1)
  ret i32 0
}

; Function Attrs: nounwind
declare dso_local i32 @printf(i8* nocapture readonly, ...) local_unnamed_addr #1

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @_Z5main3P11OuterStruct(%struct.OuterStruct* nocapture readnone) local_unnamed_addr #4 {
  %2 = load %struct.OuterStruct*, %struct.OuterStruct** @globalA, align 8, !tbaa !2
  %3 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @.str, i64 0, i64 0), %struct.OuterStruct* %2)
  ret i32 0
}

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @_Z5main2v() local_unnamed_addr #4 {
  %1 = tail call i32 @_Z5main3P11OuterStruct(%struct.OuterStruct* undef)
  ret i32 0
}

; Function Attrs: uwtable
define internal void @_GLOBAL__sub_I_global_struct.cpp() #5 section ".text.startup" {
  tail call void @_ZNSt8ios_base4InitC1Ev(%"class.std::ios_base::Init"* nonnull @_ZStL8__ioinit)
  %1 = tail call i32 @__cxa_atexit(void (i8*)* bitcast (void (%"class.std::ios_base::Init"*)* @_ZNSt8ios_base4InitD1Ev to void (i8*)*), i8* getelementptr inbounds (%"class.std::ios_base::Init", %"class.std::ios_base::Init"* @_ZStL8__ioinit, i64 0, i32 0), i8* nonnull @__dso_handle) #2
  ret void
}

attributes #0 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind }
attributes #3 = { noinline norecurse nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { noinline nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #5 = { uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 8.0.1 (tags/RELEASE_801/final)"}
!2 = !{!3, !3, i64 0}
!3 = !{!"any pointer", !4, i64 0}
!4 = !{!"omnipotent char", !5, i64 0}
!5 = !{!"Simple C++ TBAA"}
)//";

const std::string test_ir6 = R"//(
source_filename = "test"
target datalayout = "e-m:e-p:64:64-i64:64-f80:128-n8:16:32:64-S128"

@gv1 = local_unnamed_addr global i64 0
@gv2 = local_unnamed_addr global i64 0
@gv3 = constant [9 x i8] c"%d\0A%d\0A%d\00"
@gv4 = local_unnamed_addr global i64 4195648
@gv5 = local_unnamed_addr global i64 4195616
@gv6 = local_unnamed_addr global i8 0

define i64 @gf1(i32 %arg1) local_unnamed_addr {
dec_label_pc_400570:
  %0 = add i32 %arg1, 32, !insn.addr !0
  %1 = zext i32 %0 to i64, !insn.addr !1
  ret i64 %1, !insn.addr !2
}

define i64 @gf2(i32 %arg1) local_unnamed_addr {
dec_label_pc_400590:
  %0 = add i32 %arg1, 32, !insn.addr !3
  %1 = zext i32 %0 to i64, !insn.addr !4
  ret i64 %1, !insn.addr !5
}

define i64 @gf3(i32 %arg1) local_unnamed_addr {
dec_label_pc_4005c0:
  %0 = add i32 %arg1, 32, !insn.addr !6
  %1 = zext i32 %0 to i64, !insn.addr !7
  ret i64 %1, !insn.addr !8
}

define i64 @gf4(i32 %arg1) local_unnamed_addr {
dec_label_pc_400600:
  %storemerge = add i32 %arg1, 32
  %0 = zext i32 %storemerge to i64, !insn.addr !9
  ret i64 %0, !insn.addr !10
}

define i64 @gf5(i32 %arg1) local_unnamed_addr {
dec_label_pc_400660:
  %stack_var_-12.0 = add i32 %arg1, 32
  %0 = zext i32 %stack_var_-12.0 to i64, !insn.addr !11
  ret i64 %0, !insn.addr !12
}

define i64 @gf6(i64 %argc, i8** %argv) local_unnamed_addr {
dec_label_pc_400840:
  %0 = ptrtoint i8** %argv to i64
  %1 = add i64 %0, 8, !insn.addr !13
  %2 = inttoptr i64 %1 to i64*, !insn.addr !13
  %3 = load i64, i64* %2, align 8, !insn.addr !13
  %4 = inttoptr i64 %3 to i8*, !insn.addr !14
  %5 = call i32 @atoi(i8* %4), !insn.addr !14
  %6 = call i64 @gf1(i32 %5), !insn.addr !15
  %7 = call i64 @gf2(i32 %5), !insn.addr !16
  %8 = call i64 @gf3(i32 %5), !insn.addr !17
  %9 = and i64 %6, 4294967295, !insn.addr !18
  %10 = and i64 %7, 4294967295, !insn.addr !19
  %11 = and i64 %8, 4294967295, !insn.addr !20
  %12 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @gv3, i64 0, i64 0), i64 %9, i64 %10, i64 %11), !insn.addr !21
  %13 = call i64 @gf4(i32 xor (i32 ptrtoint ([9 x i8]* @gv3 to i32), i32 ptrtoint ([9 x i8]* @gv3 to i32))), !insn.addr !22
  %14 = call i64 @gf5(i32 xor (i32 xor (i32 ptrtoint ([9 x i8]* @gv3 to i32), i32 ptrtoint ([9 x i8]* @gv3 to i32)), i32 xor (i32 ptrtoint ([9 x i8]* @gv3 to i32), i32 ptrtoint ([9 x i8]* @gv3 to i32)))), !insn.addr !23
  ret i64 0, !insn.addr !24
}

declare i32 @printf(i8*, ...) local_unnamed_addr

declare i32 @atoi(i8*) local_unnamed_addr

!0 = !{i64 4195706}
!1 = !{i64 4195712}
!2 = !{i64 4195716}
!3 = !{i64 4195754}
!4 = !{i64 4195760}
!5 = !{i64 4195764}
!6 = !{i64 4195814}
!7 = !{i64 4195826}
!8 = !{i64 4195830}
!9 = !{i64 4195916}
!10 = !{i64 4195920}
!11 = !{i64 4196159}
!12 = !{i64 4196163}
!13 = !{i64 4196442}
!14 = !{i64 4196448}
!15 = !{i64 4196459}
!16 = !{i64 4196470}
!17 = !{i64 4196481}
!18 = !{i64 4196496}
!19 = !{i64 4196499}
!20 = !{i64 4196502}
!21 = !{i64 4196506}
!22 = !{i64 4196516}
!23 = !{i64 4196526}
!24 = !{i64 4196543}
)//";

const std::string test_ir7 = R"//(
; ModuleID = 'function_struct_test.bc'
source_filename = "function_struct_test.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%t2 = type { [50 x i8], i32, %t1 }
%t1 = type { [50 x i8], [50 x i8], [50 x i8], [10 x i8] }

@.str = private unnamed_addr constant [10 x i8] c"Name: %s\0A\00", align 1
@gv1 = private unnamed_addr constant [9 x i8] c"Age: %d\0A\00", align 1
@gv3 = private unnamed_addr constant [12 x i8] c"Street: %s\0A\00", align 1
@gv4 = private unnamed_addr constant [10 x i8] c"City: %s\0A\00", align 1
@gv5 = private unnamed_addr constant [11 x i8] c"State: %s\0A\00", align 1
@gv6 = private unnamed_addr constant [9 x i8] c"Zip: %s\0A\00", align 1
@__const.main.person1 = private unnamed_addr constant %t2 { [50 x i8] c"John Doe\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00", i32 30, %t1 { [50 x i8] c"123 Main St\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00", [50 x i8] c"Exampleville\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00", [50 x i8] c"CA\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00", [10 x i8] c"12345\00\00\00\00\00" } }, align 4
@.str.7 = private unnamed_addr constant [16 x i8] c"Enter new age: \00", align 1
@.str.8 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@str = private unnamed_addr constant [9 x i8] c"Address:\00", align 1

; Function Attrs: noinline nounwind uwtable
define dso_local void @gf1(%t2* byval align 8) local_unnamed_addr #0 {
  %v1 = getelementptr inbounds %t2, %t2* %0, i64 0, i32 0, i64 0
  %v2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str, i64 0, i64 0), i8* nonnull %v1)
  %v3 = getelementptr inbounds %t2, %t2* %0, i64 0, i32 1
  %v4 = load i32, i32* %v3, align 4, !tbaa !2
  %v5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @gv1, i64 0, i64 0), i32 %v4)
  %v6 = call i32 @puts(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str, i64 0, i64 0))
  %v7 = getelementptr inbounds %t2, %t2* %0, i64 0, i32 2, i32 0, i64 0
  %v8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @gv3, i64 0, i64 0), i8* nonnull %v7)
  %v9 = getelementptr inbounds %t2, %t2* %0, i64 0, i32 2, i32 1, i64 0
  %v10 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @gv4, i64 0, i64 0), i8* nonnull %v9)
  %v11 = getelementptr inbounds %t2, %t2* %0, i64 0, i32 2, i32 2, i64 0
  %v12 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @gv5, i64 0, i64 0), i8* nonnull %v11)
  %v13 = getelementptr inbounds %t2, %t2* %0, i64 0, i32 2, i32 3, i64 0
  %v14 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @gv6, i64 0, i64 0), i8* nonnull %v13)
  ret void
}

; Function Attrs: nounwind
declare dso_local i32 @printf(i8* nocapture readonly, ...) local_unnamed_addr #1

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @gf2() local_unnamed_addr #0 {
  %v1 = alloca %t2, align 8
  %v2 = getelementptr inbounds %t2, %t2* %v1, i64 0, i32 0, i64 0
  call void @llvm.lifetime.start.p0i8(i64 216, i8* nonnull %v2) #3
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* nonnull align 8 %v2, i8* align 4 getelementptr inbounds (%t2, %t2* @__const.main.person1, i64 0, i32 0, i64 0), i64 216, i1 false)
  %v3 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @.str.7, i64 0, i64 0))
  %v4 = getelementptr inbounds %t2, %t2* %v1, i64 0, i32 1
  %v5 = call i32 (i8*, ...) @__isoc99_scanf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.8, i64 0, i64 0), i32* nonnull %v4)
  call void @gf1(%t2* byval nonnull align 8 %v1)
  call void @llvm.lifetime.end.p0i8(i64 216, i8* nonnull %v2) #3
  ret i32 0
}

; Function Attrs: argmemonly nounwind
declare void @llvm.lifetime.start.p0i8(i64, i8* nocapture) #2

; Function Attrs: argmemonly nounwind
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* nocapture writeonly, i8* nocapture readonly, i64, i1) #2

; Function Attrs: nounwind
declare dso_local i32 @__isoc99_scanf(i8* nocapture readonly, ...) local_unnamed_addr #1

; Function Attrs: argmemonly nounwind
declare void @llvm.lifetime.end.p0i8(i64, i8* nocapture) #2

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @main2() local_unnamed_addr #0 {
  %1 = tail call i32 @gf2()
  ret i32 0
}

; Function Attrs: nounwind
declare i32 @puts(i8* nocapture readonly) local_unnamed_addr #3

attributes #0 = { noinline nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { argmemonly nounwind }
attributes #3 = { nounwind }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 8.0.1 (tags/RELEASE_801/final)"}
!2 = !{!3, !6, i64 52}
!3 = !{!"Person", !4, i64 0, !6, i64 52, !7, i64 56}
!4 = !{!"omnipotent char", !5, i64 0}
!5 = !{!"Simple C/C++ TBAA"}
!6 = !{!"int", !4, i64 0}
!7 = !{!"Address", !4, i64 0, !4, i64 50, !4, i64 100, !4, i64 150}
)//";

const std::string test_ir8 = R"//(
; ModuleID = 'global_struct.bc'
source_filename = "test_C_files/global_struct.cpp"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%ZZZZZZZZZZZZZZZ = type { i8 }
%aaaa = type { %bbbbbbbbbbbbbbb }
%bbbbbbbbbbbbbbb = type { i32 }

; HALLLOWOWOWOWOOWOW
; LOLOLL

@A = internal global %ZZZZZZZZZZZZZZZ zeroinitializer, align 1
@B = external hidden global i8
@C = dso_local global %aaaa { %bbbbbbbbbbbbbbb { i32 10 } }, align 4
@D = dso_local local_unnamed_addr global %aaaa* @C, align 8
@E = private unnamed_addr constant [23 x i8] c"Global Variable A: %p\0A\00", align 1
@F = appending global [1 x { i32, void ()*, i8* }] [{ i32, void ()*, i8* } { i32 65535, void ()* @_GLOBAL__sub_I_global_struct.cpp, i8* null }]

declare dso_local void @_ZNSt8ios_base4InitC1Ev(%ZZZZZZZZZZZZZZZ*) unnamed_addr #0

; Function Attrs: nounwind
declare dso_local void @_ZNSt8ios_base4InitD1Ev(%ZZZZZZZZZZZZZZZ*) unnamed_addr #1

; Function Attrs: nounwind
declare dso_local i32 @__cxa_atexit(void (i8*)*, i8*, i8*) local_unnamed_addr #2

; Function Attrs: noinline norecurse nounwind uwtable
define dso_local i32 @main() local_unnamed_addr #3 {
  %1 = load %aaaa*, %aaaa** @D, align 8, !tbaa !2
  %2 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @E, i64 0, i64 0), %aaaa* %1)
  ret i32 0
}

; HALLLOWOWOWOWOOWOW
; LOLOLL
; Function Attrs: nounwind
declare dso_local i32 @printf(i8* nocapture readonly, ...) local_unnamed_addr #1

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @_Z5main3P11OuterStruct(%aaaa* nocapture readnone) local_unnamed_addr #4 {
  %2 = load %aaaa*, %aaaa** @D, align 8, !tbaa !2
  ; HALLLOWOWOWOWOOWOW
; LOLOLL
  %3 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @E, i64 0, i64 0), %aaaa* %2) ; HALLLOWOWOWOWOOWOW
; LOLOLL
  ret i32 0
}

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @_Z5main2v() local_unnamed_addr #4 {
  %1 = tail call i32 @_Z5main3P11OuterStruct(%aaaa* undef)
  ret i32 0
}

; Function Attrs: uwtable
define internal void @_GLOBAL__sub_I_global_struct.cpp() #5 section ".text.startup" {
  tail call void @_ZNSt8ios_base4InitC1Ev(%ZZZZZZZZZZZZZZZ* nonnull @A)
  %1 = tail call i32 @__cxa_atexit(void (i8*)* bitcast (void (%ZZZZZZZZZZZZZZZ*)* @_ZNSt8ios_base4InitD1Ev to void (i8*)*), i8* getelementptr inbounds (%ZZZZZZZZZZZZZZZ, %ZZZZZZZZZZZZZZZ* @A, i64 0, i32 0), i8* nonnull @B) #2
  ret void
}

attributes #0 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind }
attributes #3 = { noinline norecurse nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { noinline nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #5 = { uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 8.0.1 (tags/RELEASE_801/final)"}
!2 = !{!3, !3, i64 0}
!3 = !{!"any pointer", !4, i64 0}
!4 = !{!"omnipotent char", !5, i64 0}
!5 = !{!"Simple C++ TBAA"}
)//";

const std::string ir8_globals_sorted[6] = {
    R"(@B = external hidden global i8)",
    R"(@D = dso_local local_unnamed_addr global %aaaa* @C, align 8)",
    R"(@A = internal global %ZZZZZZZZZZZZZZZ zeroinitializer, align 1)",
    R"(@C = dso_local global %aaaa { %bbbbbbbbbbbbbbb { i32 10 } }, align 4)",
    R"(@E = private unnamed_addr constant [23 x i8] c"Global Variable A: %p\0A\00", align 1)",
    R"(@F = appending global [1 x { i32, void ()*, i8* }] [{ i32, void ()*, i8* } { i32 65535, void ()* @_GLOBAL__sub_I_global_struct.cpp, i8* null }])"};

const std::string ir8_structs_sorted[3] = {
    R"(%ZZZZZZZZZZZZZZZ = type { i8 })",
    R"(%bbbbbbbbbbbbbbb = type { i32 })",
    R"(%aaaa = type { %bbbbbbbbbbbbbbb })"};

const std::string ir8_functions_sorted[8] = {
    R"(declare dso_local i32 @main() local_unnamed_addr)",
    R"(declare dso_local i32 @_Z5main2v() local_unnamed_addr)",
    R"(declare dso_local i32 @printf(i8*, ...) local_unnamed_addr)",
    R"(declare dso_local i32 @_Z5main3P11OuterStruct(%aaaa*) local_unnamed_addr)",
    R"(declare dso_local i32 @__cxa_atexit(void (i8*)*, i8*, i8*) local_unnamed_addr)",
    R"(declare dso_local void @_ZNSt8ios_base4InitC1Ev(%ZZZZZZZZZZZZZZZ*) unnamed_addr)",
    R"(declare dso_local void @_ZNSt8ios_base4InitD1Ev(%ZZZZZZZZZZZZZZZ*) unnamed_addr)",
    R"(declare internal void @_GLOBAL__sub_I_global_struct.cpp() section ".text.startup")"};
    
#endif