; ModuleID = 'tests/benchmark.blz'
source_filename = "tests/benchmark.blz"

; Function Attrs: nofree nosync nounwind memory(none)
define i32 @fibonacci(i32 %n) local_unnamed_addr #0 {
entry:
  %le11 = icmp slt i32 %n, 2
  br i1 %le11, label %common.ret, label %ifcont

common.ret:                                       ; preds = %ifcont, %entry
  %accumulator.tr.lcssa = phi i32 [ 0, %entry ], [ %add, %ifcont ]
  %n.tr.lcssa = phi i32 [ %n, %entry ], [ %sub6, %ifcont ]
  %accumulator.ret.tr = add i32 %n.tr.lcssa, %accumulator.tr.lcssa
  ret i32 %accumulator.ret.tr

ifcont:                                           ; preds = %entry, %ifcont
  %n.tr13 = phi i32 [ %sub6, %ifcont ], [ %n, %entry ]
  %accumulator.tr12 = phi i32 [ %add, %ifcont ], [ 0, %entry ]
  %sub = add nsw i32 %n.tr13, -1
  %call = tail call i32 @fibonacci(i32 %sub)
  %sub6 = add nsw i32 %n.tr13, -2
  %add = add i32 %call, %accumulator.tr12
  %le = icmp samesign ult i32 %n.tr13, 4
  br i1 %le, label %common.ret, label %ifcont
}

; Function Attrs: nofree norecurse nosync nounwind memory(none)
define range(i32 0, 2) i32 @isPrime(i32 %n) local_unnamed_addr #1 {
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
define i32 @countPrimes(i32 %n) local_unnamed_addr #1 {
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
define i32 @factorial(i32 %n) local_unnamed_addr #1 {
entry:
  %le.not11 = icmp slt i32 %n, 2
  br i1 %le.not11, label %whileend, label %whilebody

whilebody:                                        ; preds = %entry, %whilebody
  %i.013 = phi i32 [ %add, %whilebody ], [ 2, %entry ]
  %result.012 = phi i32 [ %mul, %whilebody ], [ 1, %entry ]
  %mul = mul i32 %i.013, %result.012
  %add = add i32 %i.013, 1
  %le.not = icmp sgt i32 %add, %n
  br i1 %le.not, label %whileend, label %whilebody

whileend:                                         ; preds = %whilebody, %entry
  %result.0.lcssa = phi i32 [ 1, %entry ], [ %mul, %whilebody ]
  ret i32 %result.0.lcssa
}

; Function Attrs: nofree norecurse nosync nounwind memory(none)
define i32 @sum(i32 %n) local_unnamed_addr #1 {
entry:
  %le.not12 = icmp slt i32 %n, 1
  br i1 %le.not12, label %whileend, label %whilebody

whilebody:                                        ; preds = %entry, %whilebody
  %i.014 = phi i32 [ %add7, %whilebody ], [ 1, %entry ]
  %total.013 = phi i32 [ %add, %whilebody ], [ 0, %entry ]
  %add = add i32 %i.014, %total.013
  %add7 = add i32 %i.014, 1
  %le.not = icmp sgt i32 %add7, %n
  br i1 %le.not, label %whileend, label %whilebody

whileend:                                         ; preds = %whilebody, %entry
  %total.0.lcssa = phi i32 [ 0, %entry ], [ %add, %whilebody ]
  ret i32 %total.0.lcssa
}

; Function Attrs: nofree norecurse nosync nounwind memory(none)
define i32 @gcd(i32 %a, i32 %b) local_unnamed_addr #1 {
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
define i32 @power(i32 %base, i32 %exponent) local_unnamed_addr #1 {
entry:
  %lt11 = icmp sgt i32 %exponent, 0
  br i1 %lt11, label %whilebody, label %whileend

whilebody:                                        ; preds = %entry, %whilebody
  %i.013 = phi i32 [ %add, %whilebody ], [ 0, %entry ]
  %result.012 = phi i32 [ %mul, %whilebody ], [ 1, %entry ]
  %mul = mul i32 %result.012, %base
  %add = add nuw nsw i32 %i.013, 1
  %lt = icmp slt i32 %add, %exponent
  br i1 %lt, label %whilebody, label %whileend

whileend:                                         ; preds = %whilebody, %entry
  %result.0.lcssa = phi i32 [ 1, %entry ], [ %mul, %whilebody ]
  ret i32 %result.0.lcssa
}

; Function Attrs: nofree norecurse nosync nounwind memory(none)
define i32 @collatz(i32 %n) local_unnamed_addr #1 {
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

attributes #0 = { nofree nosync nounwind memory(none) }
attributes #1 = { nofree norecurse nosync nounwind memory(none) }
