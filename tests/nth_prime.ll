; ModuleID = 'tests/nth_prime.blz'
source_filename = "tests/nth_prime.blz"

; Function Attrs: nofree norecurse nosync nounwind memory(none)
define i32 @nthPrime(i32 %n) local_unnamed_addr #0 {
entry:
  %lt32 = icmp sgt i32 %n, 0
  br i1 %lt32, label %whilebody, label %whileend20

whilebody:                                        ; preds = %entry, %whileend
  %count.034 = phi i32 [ %spec.select28, %whileend ], [ 0, %entry ]
  %num.033 = phi i32 [ %add, %whileend ], [ 1, %entry ]
  %add = add i32 %num.033, 1
  %lt829 = icmp sgt i32 %add, 2
  br i1 %lt829, label %whilebody9, label %whileend

whilebody9:                                       ; preds = %whilebody, %whilebody9
  %isPrime.031 = phi i32 [ %spec.select, %whilebody9 ], [ 1, %whilebody ]
  %divisor.030 = phi i32 [ %add13, %whilebody9 ], [ 2, %whilebody ]
  %mod = urem i32 %add, %divisor.030
  %eq = icmp eq i32 %mod, 0
  %spec.select = select i1 %eq, i32 0, i32 %isPrime.031
  %add13 = add nuw nsw i32 %divisor.030, 1
  %lt8 = icmp slt i32 %add13, %add
  br i1 %lt8, label %whilebody9, label %whileend

whileend:                                         ; preds = %whilebody9, %whilebody
  %isPrime.0.lcssa = phi i32 [ 1, %whilebody ], [ %spec.select, %whilebody9 ]
  %spec.select28 = add i32 %isPrime.0.lcssa, %count.034
  %lt = icmp slt i32 %spec.select28, %n
  br i1 %lt, label %whilebody, label %whileend20

whileend20:                                       ; preds = %whileend, %entry
  %num.0.lcssa = phi i32 [ 1, %entry ], [ %add, %whileend ]
  ret i32 %num.0.lcssa
}

attributes #0 = { nofree norecurse nosync nounwind memory(none) }
