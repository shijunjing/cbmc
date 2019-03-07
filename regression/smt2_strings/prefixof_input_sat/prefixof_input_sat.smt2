(declare-const in1 String)
(declare-const in2 String)
(assert (str.prefixof in1 in2))
(check-sat)
