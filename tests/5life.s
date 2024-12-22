(d w 1280)
(d h 600)

(d block 10)
(d bw (divi w block))
(d bh (divi h block))

;sin(x) ≈ 4x(180 - x)/(40500 - x(180 - x))
(d sin
 (l (x)
  (d a (sub 180 x))
  (divf
   (itof (mul (mul 4 x) a))
   (itof (sub 40500 (mul x a))))))

(d zrow (fill bw (l () 0)))
(d world
 (fill bh
  (l (y)
   (fill bw
    (l (x)
     (mod (ftoi (divf (sin (sum (sin y) x))
                 0.0017))
      2))))))

(d nextCell
 (l (curr m n o p q r s t)
  (d value (fold (m n o p q r s t) 0 sum))
  (if (or (less value 2) (more value 3)) 0
   (if (ieq value 3) 1 curr))))
 
(d nextRow
 (l (r0 r1 r2)
  (if (nil? (cdr (cdr r1))) (0)
   (cons (nextCell
          (cadr r1) (car r1) (car (cddr r1))
          (car r2) (cadr r2) (car (cddr r2))
          (car r0) (cadr r0) (car (cddr r0)))
    (nextRow (cdr r0) (cdr r1) (cdr r2))))))

(d nextWorld
 (l (head world)
  (if (nil? (cdr world)) (zrow)
   (cons
    (cons 0 (nextRow head
             (car world)
             (cadr world)))
    (nextWorld (car world) (cdr world))))))
  
(d drawRow
 (l (row x y)
  (d cont
   (l ()
    (if (car row)
     (DrawRectangle
      (mul x block)
      (mul y block)
      block block #FF556688) ())
    (drawRow (cdr row) (sum x 1) y)))
  (if (nil? row) () (cont))))

(d drawWorld
 (l (world y)
  (d cont
   (l ()
    (drawRow (car world) 0 y)
    (drawWorld (cdr world) (sum y 1))))
  (if (nil? world) () (cont))))

(d draw (l (world)
    (BeginDrawing)
        (ClearBackground #FFAAFFFF)
        (drawWorld world 0)
    (EndDrawing)

    (if (WindowShouldClose) ()
     (draw
      (cons zrow
       (nextWorld
            (car world)
            (cdr world)))))))

(InitWindow w h "life")
(draw world)
