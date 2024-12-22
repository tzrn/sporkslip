(d width 800)
(d height 600)

(d block_size 50)
(d bw (div width block_size))
(d bh (div height block_size))
(d half (divi block_size 2))
(d cenx (sub (divi width  2) half))
(d ceny (sub (divi height 2) half))

(d kmove
 (l (val ksum kmin)
  (if (IsKeyPressed kmin)
   (sub val 1)
  (if (IsKeyPressed ksum)
   (sum val 1) val))))

(d check_col
 (l (block px py)
  (and
   (ieq px (car block))
   (ieq py (cdr block)))))

(d DrawBlock
 (l (block cx cy)
  (DrawRectangle
   (sum cx (mul (car block) block_size))
   (sum cy (mul (cdr block) block_size))
   block_size block_size
   (ColorFromHSV
    (itof 
     (mul 
      (sum (car block) (cdr block)) 10))
    0.7 1.0))))

;player and objs coords are in blocks,
;camera is in pixels
(d draw
 (l (px py cx cy objs)

  (BeginDrawing)
    (ClearBackground #FFFFFF)
    (DrawRectangle
     (sum cx (mul px block_size))
     (sum cy (mul py block_size))
     block_size block_size
     #FF0000FF)
    (map (l (b) (DrawBlock b cx cy)) objs)
  (EndDrawing)

  (d camv
   (l (curr cen p)
    (sub (- (divi
             (sub curr cen)
             block_size)) p)))

  (d nextpx (kmove px KEY_D KEY_A))
  (d nextpy (kmove py KEY_S KEY_W))

  (d collision
   (fold objs 0
    (l (new old)
     (sum
      (check_col new nextpx nextpy)
      old))))

  (if (WindowShouldClose) ()
   (draw
    (if collision px nextpx)
    (if collision py nextpy)
    
    (sum cx (camv cx cenx px))
    (sum cy (camv cy ceny py))

    (if (IsKeyPressed KEY_SPACE)
     (cons (cons px (sum py 1)) objs)
    (if (IsKeyPressed KEY_J)
     (filter objs
      (l (obj)
       (not (and
        (ieq nextpx (car obj))
        (ieq (sum 1 nextpy) (cdr obj))))))
     objs))))))

(InitWindow width height "Hello world")
(SetTargetFPS 60)

(draw 0 0 cenx ceny ())

(CloseWindow)
