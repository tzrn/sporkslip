(d width 800)
(d height 600)

(d ballSpeed   8)
(d padSpeed    4)
(d padw      100)
(d padh       30)
(d ballsize   20)
(d startx
 (sub (divi width 2)
      (divi padw  2)))

(d padtop (sub height padh))
(d fballsize (itof ballsize))

(d draw
 (l (ballx bally ballvx ballvy padx pady points)
  (d padright (sum padx padw))
  (d ballleft (sum ballx ballsize))

 (d collision
  (and (and
    (more ballx padx)
    (less ballx padright))
   (more (sum bally ballsize) pady)))
 (if collision (PlaySound2 boom) ())

 (if (IsSoundPlaying bgm) ()
  (PlaySound2 bgm))

  (BeginDrawing)
    (DrawTextureEx2 bg
     0 0 0.0 1.0 #FFFFFFFF)
    (DrawCircle ballx bally fballsize
     (if collision #FF0000FF 
      (ColorFromHSV (itof (mul points 30)) 1.0 1.0)))
    (DrawRectangle padx pady
     padw padh
     (ColorFromHSV
      (itof (sub 360 (divi bally 10))) 1.0 1.0))
    (DrawText
     (TextFormat "score: %d" points)
     3 0 40 #FF000000)
  (EndDrawing)
 
 (if (WindowShouldClose) ()
 (if (more bally (sum height ballsize))
  (end "GAME OVER, SPACE to retry")
 (draw
   (sum ballx ballvx)
   (if collision
    (sub padtop ballsize)
    (sum bally ballvy))

   (if (or
        (more (sum ballx ballsize) width)
        (less (sub ballx ballsize) 0))
    (- ballvx)
    ballvx)

   (if collision
    (- ballSpeed)
    (if (less (sub bally ballsize) 0)
     ballSpeed
     ballvy))

  (if (and (IsKeyDown KEY_A)
       (more padx 0))
   (sub padx padSpeed)
  (if (and (IsKeyDown KEY_D)
       (less padright width))
   (sum padx padSpeed)
   padx)) pady (sum points collision))))))

(InitWindow width height "Hello world")
(InitAudioDevice)
(SetTargetFPS 60)

(d boom (LoadSound2    "res/vine-boom.mp3"    ))
(d bgm  (LoadSound2    "res/bgm.wav"          ))
(d bg   (LoadTextureEx "res/bg2.png" 800 600 0))

(d end (l (text)
  (BeginDrawing)
    (DrawTextureEx2 bg 0 0 0.0 1.0 #FFFFFFFF)
    (DrawText text 100 100 40 #FF000000)
  (EndDrawing)
  
  (if (WindowShouldClose) ()
   (if (IsKeyDown KEY_SPACE) (game)
    (end text)))))

(d game (l () (draw startx 600
      ballSpeed ballSpeed
      startx (sub height padh) (- 1))))

(end "Press SPACE to start :)")
(CloseWindow)
