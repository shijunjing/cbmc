(declare-const a String)
(declare-const b String)
(declare-const c String)
(assert (= (str.++ a "dc" b c) "abcdefgh"))
(check-sat)
