(d isdig
 (l (char)
  (and
   (more char 47)
   (less char 58))))

(d scannum
 (l (str num ind)
  (d char (sref str ind))
  (if (isdig char)
   (scannum str
    (sum
     (sub char 48)
     (mul num 10))
    (sum ind 1))
   (cons num ind))))

(d scannums
 (l (str nums ind)
  (d cont
   (l ()
    (d num (scannum str 0 ind))
    (scannums str
     (cons (car num) nums)
     (cdr num))))

  (d char (sref str ind))
  (if (ieq char 0) (reverse nums)
   (if (isdig char) (cont)
    (scannums str nums (sum ind 1))))))

(d menu (readf "res/menu"))
(print menu NL
 "Sum - "
 (fold (scannums menu () 0) 0 sum)
 "kk" NL)
