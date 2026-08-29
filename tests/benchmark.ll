; ModuleID = 'tests/benchmark.blz'
source_filename = "tests/benchmark.blz"

; Function Attrs: nofree norecurse nosync nounwind memory(none)
define i32 @fibonacci(i32 %n) local_unnamed_addr #0 {
entry:
  %le = icmp slt i32 %n, 2
  br i1 %le, label %common.ret, label %whilebody

common.ret:                                       ; preds = %whilebody, %entry
  %common.ret.op = phi i32 [ %n, %entry ], [ %add, %whilebody ]
  ret i32 %common.ret.op

whilebody:                                        ; preds = %entry, %whilebody
  %i.021 = phi i32 [ %add12, %whilebody ], [ 2, %entry ]
  %a.020 = phi i32 [ %b.019, %whilebody ], [ 0, %entry ]
  %b.019 = phi i32 [ %add, %whilebody ], [ 1, %entry ]
  %add = add i32 %a.020, %b.019
  %add12 = add i32 %i.021, 1
  %le6.not = icmp sgt i32 %add12, %n
  br i1 %le6.not, label %common.ret, label %whilebody
}

; Function Attrs: nofree norecurse nosync nounwind memory(none)
define range(i32 0, 2) i32 @isPrime(i32 %n) local_unnamed_addr #0 {
entry:
  %lt = icmp slt i32 %n, 2
  br i1 %lt, label %common.ret, label %whilecond.preheader

whilecond.preheader:                              ; preds = %entry
  %le.not16 = icmp samesign ult i32 %n, 4
  br i1 %le.not16, label %common.ret, label %whilebody

common.ret:                                       ; preds = %whilebody, %whilecond, %whilecond.preheader, %entry
  %common.ret.op = phi i32 [ 0, %entry ], [ 1, %whilecond.preheader ], [ 0, %whilebody ], [ 1, %whilecond ]
  ret i32 %common.ret.op

whilecond:                                        ; preds = %whilebody
  %add = add i32 %i.017, 1
  %mul = mul i32 %add, %add
  %le.not = icmp sgt i32 %mul, %n
  br i1 %le.not, label %common.ret, label %whilebody

whilebody:                                        ; preds = %whilecond.preheader, %whilecond
  %i.017 = phi i32 [ %add, %whilecond ], [ 2, %whilecond.preheader ]
  %mod = srem i32 %n, %i.017
  %eq = icmp eq i32 %mod, 0
  br i1 %eq, label %common.ret, label %whilecond
}

; Function Attrs: nofree norecurse nosync nounwind memory(none)
define i32 @countPrimes(i32 %n) local_unnamed_addr #0 {
entry:
  %le.not16 = icmp slt i32 %n, 2
  br i1 %le.not16, label %whileend, label %whilebody

whilebody:                                        ; preds = %entry, %whilecond
  %i.018 = phi i32 [ %add8, %whilecond ], [ 2, %entry ]
  %count.017 = phi i32 [ %0, %whilecond ], [ 0, %entry ]
  %lt.i = icmp slt i32 %i.018, 2
  br i1 %lt.i, label %whilecond, label %whilecond.preheader.i

whilecond.preheader.i:                            ; preds = %whilebody
  %le.not16.i = icmp samesign ult i32 %i.018, 4
  br i1 %le.not16.i, label %isPrime.exit, label %whilebody.i

whilecond.i:                                      ; preds = %whilebody.i
  %add.i = add i32 %i.017.i, 1
  %mul.i = mul i32 %add.i, %add.i
  %le.not.i = icmp sgt i32 %mul.i, %i.018
  br i1 %le.not.i, label %isPrime.exit, label %whilebody.i

whilebody.i:                                      ; preds = %whilecond.preheader.i, %whilecond.i
  %i.017.i = phi i32 [ %add.i, %whilecond.i ], [ 2, %whilecond.preheader.i ]
  %mod.i = srem i32 %i.018, %i.017.i
  %eq.i = icmp eq i32 %mod.i, 0
  br i1 %eq.i, label %whilecond, label %whilecond.i

isPrime.exit:                                     ; preds = %whilecond.i, %whilecond.preheader.i
  %add = add i32 %count.017, 1
  br label %whilecond

whilecond:                                        ; preds = %whilebody.i, %whilebody, %isPrime.exit
  %0 = phi i32 [ %add, %isPrime.exit ], [ %count.017, %whilebody ], [ %count.017, %whilebody.i ]
  %add8 = add i32 %i.018, 1
  %le.not = icmp sgt i32 %add8, %n
  br i1 %le.not, label %whileend, label %whilebody

whileend:                                         ; preds = %whilecond, %entry
  %count.0.lcssa = phi i32 [ 0, %entry ], [ %0, %whilecond ]
  ret i32 %count.0.lcssa
}

; Function Attrs: nofree norecurse nosync nounwind memory(none)
define i32 @gcd(i32 %a, i32 %b) local_unnamed_addr #0 {
entry:
  %neq.not14 = icmp eq i32 %b, 0
  br i1 %neq.not14, label %whileend, label %whilebody

whilebody:                                        ; preds = %entry, %whilebody
  %y.016 = phi i32 [ %mod, %whilebody ], [ %b, %entry ]
  %x.015 = phi i32 [ %y.016, %whilebody ], [ %a, %entry ]
  %mod = srem i32 %x.015, %y.016
  %neq.not = icmp eq i32 %mod, 0
  br i1 %neq.not, label %whileend, label %whilebody

whileend:                                         ; preds = %whilebody, %entry
  %x.0.lcssa = phi i32 [ %a, %entry ], [ %y.016, %whilebody ]
  ret i32 %x.0.lcssa
}

; Function Attrs: nofree norecurse nosync nounwind memory(none)
define i32 @collatz(i32 %n) local_unnamed_addr #0 {
entry:
  %neq.not14 = icmp eq i32 %n, 1
  br i1 %neq.not14, label %whileend, label %whilebody

whilebody:                                        ; preds = %entry, %whilebody
  %steps.016 = phi i32 [ %add8, %whilebody ], [ 0, %entry ]
  %x.015 = phi i32 [ %x.1, %whilebody ], [ %n, %entry ]
  %0 = and i32 %x.015, 1
  %eq = icmp eq i32 %0, 0
  %div = ashr exact i32 %x.015, 1
  %mul = mul i32 %x.015, 3
  %add = add i32 %mul, 1
  %x.1 = select i1 %eq, i32 %div, i32 %add
  %add8 = add i32 %steps.016, 1
  %neq.not = icmp eq i32 %x.1, 1
  br i1 %neq.not, label %whileend, label %whilebody

whileend:                                         ; preds = %whilebody, %entry
  %steps.0.lcssa = phi i32 [ 0, %entry ], [ %add8, %whilebody ]
  ret i32 %steps.0.lcssa
}

attributes #0 = { nofree norecurse nosync nounwind memory(none) }
