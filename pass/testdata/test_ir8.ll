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
