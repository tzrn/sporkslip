(d width  1200)
(d height 600)
	          
(d drawobj (l (obj)
	(d x     (car obj))
	(d y     (car (cdr obj)))
	(d prop  (car (cdr (cdr obj))))
	(d size  (car prop))
	(d speed (car (cdr prop)))
	
	(d nextv (l (coord max)
		(d pos (car coord))
		(d edge (sum pos size))
		(if (ieq pos size) speed
			(if (ieq edge max) (sub 0 speed)
				(car (cdr coord))))))

	(DrawCircle (car x)
			    (car y)
			    (itof (car prop))
				(ColorFromHSV (itof size) .8 1.0))
				
	(d nextvx (nextv x width ))
	(d nextvy (nextv y height))
	(((sum (car x) nextvx) nextvx)
	 ((sum (car y) nextvy) nextvy) prop) ))
	

(d draw (l (os)
	(BeginDrawing)
		(ClearBackground #FFAAFFFF)
		(d next (map drawobj os))
		(DrawFPS 0 0)
	(EndDrawing)
	
	(if (band (IsKeyPressed KEY_SPACE) 1)
		(ToggleFullscreen) 0)
	
	(if (WindowShouldClose)
	    0
	    (draw next))))

(InitWindow width height "test")
(SetTargetFPS 60)
;        x vx  y  vy   r   V
(draw (((0 1) (100 1) (100 1))
       ((600 1) (0 1) (75 2))
       ((0 1)   (0 1) (60 2))
       ((300 1) (0 1) (50 4))
       ((0 1) (200 1) (40 4))
       ((0 1) (300 1) (10 5))))
(CloseWindow)
