(d w 800)
(d h 600)
(d boxh 150)

(d STR 0)
(d WORD 1)

(d script
 (readf "res/ks/script-a1-monday.rpy"))

(d isspace
 (l (char)
  (or (ieq char 32)
      (ieq char 9))))

(d skipspace
 (l (str n)
  (if
   (isspace
    (sref str n))
   (skipspace str (sum n 1))
   n)))

(d skipspacenl
 (l (str n)
  (d curr (sref str n))
  (if
   (or (isspace curr)
       (ieq curr 10))
   (skipspacenl str (sum n 1)) n)))

(d scanword
 (l (str start end)
  (d curr (sref str end))
  (if (or
       (or (isspace curr)
           (ieq curr 10))
       (ieq curr 0))
   (cons
    (slice str start end)
    end)
   (scanword str
    start (sum end 1)))))

(d break_text'
 (l (text lines start end len)
  (if (ieq (sref text end) 0)
   (reverse
    (cons (slice text start end) lines))
  (if (ieq (mod end len) 0)
   (break_text' text 
    (cons (slice text start end len)
     lines)
    end (sum end 1) len)
   (break_text' text lines
    start (sum end 1) len)))))

(d break_text
 (l (text)
  (break_text' text () 0 1 70)))

(d scanstr
 (l (str start end)
  (d curr (sref str end))

  (if (ieq curr 92) ;\
   (scanstr str start (sum end 2))
  (if (ieq curr 34) ;"
   (cons
    (break_text (slice str start end))
    (sum end 1))
   (scanstr str start (sum end 1))))))

(d scanline
 (l (str n words)
  (d curr (sref str n))

  (d cont
   (l (func n t)
    (d newn (skipspace str n))
    (d res (func str newn newn))
    (scanline str (skipspace str (cdr res))
     (cons (cons t (car res)) words))))
  
  (if (or (ieq curr 0)   ;EOF
          (ieq curr 10)) ;NL
   (cons
    (reverse words)
    (skipspace str n))
  (if (ieq curr 34) ;"
   (cont scanstr (sum n 1) STR)
   (cont scanword n WORD)))))

(d scanlines
 (l (str n lines)
  (d curr (sref str n))

  (d cont
   (l ()
    (d line
     (scanline
      str
      (skipspacenl str n)
      ()))
    (scanlines str (cdr line)
     (cons (car line) lines))))

  (if (ieq curr 0) ;EOF
   (reverse lines)
   (cont))))

(d seq
 (l (str1 str2 n)
  (if
   (ieq
    (sref str1 n)
    (sref str2 n))
   (if (ieq (sref str1 n) 0) 1
    (seq str1 str2 (sum n 1)))
   0)))

(d mapf
 (l (map key)
  (if (nil? map) ()
  (if (seq (car (car map)) key 0)
   (cdr (car map))
   (mapf (cdr map) key)))))

(d comp1
 (l (s1 s2)
  (less (sref (car s1) 0)
        (sref (car s2) 0))))

(d draw
 (l (cmds textures bg bgm bgms text
     csprites sprites)
  (d cmd (car cmds))

 (if (nil? bgm) ()
  (UpdateMusicStream2 bgm))

  (d skip
   (l ()
    (draw (cdr cmds)
     textures bg bgm bgms text
     csprites sprites)))

  (d cont_newbg
   (l (cmd)
    (d folder (concat "res/ks/" (cdr (car cmd))))
    (d file   (cdr (car (cdr cmd))))
    (d filename (concat folder
                 (concat "s/"
                  (concat file ".png"))))
    (d tex (mapf textures filename))
    (d newtex
     (if (nil? tex)
      (LoadTexture2 filename) tex))
    
    (draw
     (cdr cmds)
     (if (nil? tex)
      (cons (cons filename newtex) textures)
      textures)
     newtex bgm bgms text () sprites)))

  (d cont_newbgm
   (l (cmd)
    (d folder (cdr (car cmd)))
    (d music? (seq folder "music" 0))
    (d folder
     (concat (if music? "bgm" "sfx") "/"))
    (d name
     (concat
      (cdr (car (cdr cmd)))
      ".ogg"))
    (d filename
     (concat
      (concat "res/ks/" folder) name))
    (d mbgm (mapf bgms filename))
    (d newbgm
     (if (nil? mbgm)
       (LoadMusicStream2 filename)
       bgm))

    (d contmusic
     (l ()

    (PlayMusicStream2 newbgm)

    (print filename NL)
    (draw (cdr cmds)
     textures
     bg newbgm
     (if (nil? bgm)
      (cons (cons filename bgm) bgms)
      bgms) text csprites sprites)))

    (if music? (contmusic) (skip))))

  (d cont_sprite
   (l (args)
    (d char (cdr (car args)))
    (d folder 
     (concat "res/ks/sprites/"
      (concat char "/")))
    (d pose (cdr (car (cdr args))))
    (d filename
     (concat (concat char "_")
      (concat pose ".png")))
    (d loc (concat folder filename))

    (d sprite (mapf sprites loc))
    (d newsprite
     (if (nil? sprite)
      (LoadTexture2 loc) sprite))

    (draw (cdr cmds) textures bg bgm
     bgms text

    (sort
     (cons (cons char newsprite)
      (filter csprites
       (l (spr)
        (not (seq (car spr) char 0)))))
     comp1)

     (if (nil? sprite)
      (cons
       (cons loc newsprite) sprites)
      sprites))))
    
  (d cont_hide
   (l (args)
    (d char (cdr (car args)))
    (draw (cdr cmds) textures bg
     bgm bgms text
     (filter csprites
      (l (spr)
       (not (seq (car spr) char 0))))
     sprites)))

  (d draw_text
   (l (text y)

    (d cont
     (l ()
      (DrawText (car text)
       10 (sum (sub h boxh) y) 20 #FFFFFFFF)
      (draw_text (cdr text) (sum y 30))))

    (if (nil? text) ()
     (cont))))

  (d c (count csprites))
  (d chunk
   (if (more c 0)
    (divi h c) 1))
  (d halfchunk (divi chunk 2))

  (d drawSprites
   (l (sprites n)
    (d cont
     (l ()
      (d sprite (cdr (car sprites)))
      (DrawTextureEx2 sprite
       (sum (mul n chunk) halfchunk)
       0 0.0 1.0 #FFFFFFFF)
      (drawSprites (cdr sprites) (sum n 1))))

    (if (nil? sprites) ()
     (cont))))

  (BeginDrawing)
    (if (nil? bg)
        (ClearBackground #FF000000)
        (DrawTextureEx2
         bg (- 100) 0 0.0 1.0 #FFFFFFFF))
    (drawSprites csprites 0)
    (DrawRectangle
     0 (sub h boxh) w boxh #88000000)
    (draw_text text 0)
    (DrawFPS 0 0)
  (EndDrawing)

  (d cont_cmd
   (l (head args)
    (if (seq head "scene" 0)
     (cont_newbg (cdr cmd))
    (if (seq head "play" 0)
     (cont_newbgm (cdr cmd))
    (if (seq head "show" 0)
     (cont_sprite (cdr cmd))
    (if (seq head "hide" 0)
     (cont_hide (cdr cmd))
    (if (ieq (car (car args)) STR)
     (draw 
      (cons
       ((cons STR
        (cons head
         (cdr
          (car
           (cdr
            (car cmds)))))))
       (cdr cmds))
      textures bg bgm bgms
      text csprites sprites)
    (skip))))))))

  (d next
   (or
    (IsKeyPressed KEY_SPACE)
    (IsKeyDown KEY_W)))
  (d head (car cmd))

  (if (IsKeyPressed KEY_J)
   (ToggleFullscreen) ())

   (if (ieq (car head) WORD)
     (cont_cmd (cdr head) (cdr (car cmds)))
    (draw (if next (cdr cmds) cmds)
     textures bg bgm bgms (cdr head)
     csprites sprites))))


(InitAudioDevice)
(InitWindow w h "ks")
(SetTargetFPS 30)
(draw
 (scanlines script 0 ()) ;cmds
 () ;textures
 () ;bg
 () ;bgm
 () ;bgms
 () ;text
 () ;csprites
 () ;sprites
 )
(CloseWindow)
