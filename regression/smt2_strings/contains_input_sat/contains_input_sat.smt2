(declare-const needle String)
(declare-const haystack String)
(assert (str.contains haystack needle))
(check-sat)
