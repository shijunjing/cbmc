(declare-const haystack String)
(declare-const needle String)
(declare-const start Int)
(declare-const index Int)
(assert (= (str.indexof haystack needle start) index))
(check-sat)
