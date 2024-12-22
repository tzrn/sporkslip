(d w 800)
(d h 600)
(d b 50)
(d bw (divi w b))
(d bh (divi h b))

(d pieq
 (l (p1 p2)
  (and (ieq (car p1) (car p2))
   (ieq (cdr p1) (cdr p2)))))

(d isDead
 (l (head body)
  (if (nil? body) 0
   (if (pieq head (car body)) 1
    (isDead head (cdr body))))))

(d drawBlock
 (l (pos color)
  (DrawRectangle
   (mul b (car pos))
   (mul b (cdr pos)) b b
   color)))

(d wrap
 (l (num min max)
  (if (more num max) min
   (if (less num min) max
    num))))

(d drawSnake
 (l (snake)
  (d cont
   (l ()
    (drawBlock (car snake) #FF00FF00)
    (drawSnake (cdr snake))))
  (if (nil? snake) ()
   (cont))))

(d remlast
 (l (lst)
  (if (nil? (cdr lst)) ()
   (cons
    (car lst)
    (remlast (cdr lst))))))

(d draw
 (l (snake vel apple)
  (BeginDrawing)
   (ClearBackground #FFFFFFFF)
   (drawBlock apple #FF0000FF)
   (drawSnake snake)
  (EndDrawing)

  (d head (car snake))
  (d eat (pieq head apple))
  (d dead (isDead head (cdr snake)))

  (d newhead
   (cons
    (wrap (sum (car head)
           (car vel)) 0 (sub bw 1))
    (wrap (sum (cdr head)
           (cdr vel)) 0 (sub bh 1))))

  (d new
   (if dead (newhead)
    (cons newhead
     (if eat snake (remlast snake)))))

  (if (WindowShouldClose) ()
   (draw new 
    (if (IsKeyDown KEY_A)
     (cons (- 1) 0)
    (if (IsKeyDown KEY_D)
     (cons 1 0)
    (if (IsKeyDown KEY_W)
     (cons 0 (- 1))
    (if (IsKeyDown KEY_S)
     (cons 0 1) vel))))
    (if eat
     (cons
      (wrap (sub (car apple) 3) 1 (sub bw 1))
      (wrap (sub (cdr apple) 8) 1 (sub bh 1)))
     apple)))))

(InitWindow w h "test 12")
(SetTargetFPS 10)
(draw ((cons 0 0)) (cons 1 0) (cons 10 10))
(CloseWindow)
