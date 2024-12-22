(d lib (ld "libs/lib.so"))

(d NL 
"
")

(f lib sum  int int int)
(f lib sub  int int int)
(f lib divi int int int)
(f lib divf float float float)
(f lib mod  int int int)
(f lib mul  int int int)
(f lib more int int int)
(f lib less int int int)
(f lib band int int int)
(f lib ieq  int int int)
(f lib and  int int int)
(f lib or   int int int)
(f lib not int int)

(f lib print_int int void)
(f lib print_float float void)
(f lib print_str ptr void)

(f lib itof int float)
(f lib ftoi float int)

(f lib sys ptr void)
(f lib eep int void)
(f lib sref ptr int int)
(f lib readf ptr gstr)
(f lib slice ptr int int gstr)
(f lib concat ptr ptr gstr)

(d cadr (l (p) (car (cdr p))))
(d cddr (l (p) (cdr (cdr p))))

(d - (l (x) (sub 0 x)))

(d print (l () 
    (d print_list (l (lst)
		(d cont (l (fnc)
			(fnc (car lst))
			(print_list (cdr lst))))
	
		(if (nil? lst) ()
		(if (float? (car lst)) (cont print_float)
        (if (int?   (car lst)) (cont print_int)
		(if (str?   (car lst)) (cont print_str)
		(if (pair?  (car lst)) (cont print_pair)
		())))))))
		
	(print_list ...)))

(d print_pair
 (l (pair)
  (if (nil? pair) ()
  (print "(" (car pair) " . " (cdr pair) ")"))))

(d map (l (fnc lst)
	(if (nil? lst) ()
	    (cons (fnc (car lst))
	          (map fnc (cdr lst))))))

(d print_list'
 (l (lst)
  (d pr_sp (l (v) (print v " " NL)))
  (print "( ")
  (map pr_sp lst)
  (print ")" NL)))

(d print_list
 (l (lst)
  (map print lst)))

(d lref (l (lst ind)
	(if (ieq ind 0) (car lst)
	    (lref (cdr lst) (sub ind 1)))))

(d reverse'
 (l (lst new)
   (d x (car lst))
   (if (nil? lst) new
    (reverse' (cdr lst) (cons x new)))))

(d reverse (l (lst) (reverse' lst ())))

(d join
 (l (l1 l2)
  (if (nil? l1) l2
   (cons
    (car l1)
    (join (cdr l1) l2)))))

(d filter
 (l (lst fnc)
  (if (nil? lst) ()
   (if (fnc (car lst))
    (cons (car lst) (filter (cdr lst) fnc))
    (filter (cdr lst) fnc)))))

(d filter2
 (l (lst fnc)
  (d filter2'
   (l (lst l1 l2 fnc)
    (d cont (l ()
      (d n (car lst))
      (d x (fnc n))
      (filter2' (cdr lst)
       (if x (cons n l1) l1)
       (if x l2 (cons n l2)) fnc)))
    (if (nil? lst)
     (cons l1 l2)
     (cont))))
  (filter2' lst () () fnc)))

(d sort (l (lst comp)
 (d sort'
  (l (lst comp)
   (d head (car lst))
      (d spl (filter2
       (cdr lst)
       (l (x) (comp head x))))
      (join
       (sort (car spl) comp)
       (cons head (sort (cdr spl) comp)))))
 (if (nil? lst) ()
  (sort' lst comp))))

(d fold
 (l (lst start fnc)
  (if (nil? lst) start
   (fnc
    (car lst)
    (fold (cdr lst) start fnc)))))

(d fill
 (l (n func)
  (if (ieq n 0) ()
   (cons
    (func n)
    (fill (sub n 1) func)))))

(d count
 (l (lst)

  (d count'
   (l (lst n)
    (if (nil? lst) n
     (count'
      (cdr lst)
      (sum n 1)))))

  (count' lst 0)))
