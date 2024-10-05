int Game::rectsCollision(SDL_Rect& smallRect, SDL_Rect& bigRect)
{
	smallRectPoints[0] = {smallRect.x, smallRect.y};
	smallRectPoints[1] = {smallRect.x, smallRect.y + smallRect.h};
	smallRectPoints[2] = {smallRect.x + smallRect.w, smallRect.y};
	smallRectPoints[3] = {smallRect.x + smallRect.w, smallRect.y + smallRect.h};

	bigRectPoints[0] = {bigRect.x, bigRect.y};
	bigRectPoints[1] = {bigRect.x + bigRect.w, bigRect.y + bigRect.h};

	for (int i = 0; i < 4; i++)
	{
		if (smallRectPoints[i].x >= bigRectPoints[0].x && smallRectPoints[i].x <= bigRectPoints[1].x && smallRectPoints[i].y >= bigRectPoints[0].y && smallRectPoints[i].y <= bigRectPoints[1].y) 
		{
			return 1;
		}
	}
	return 0;
}
//test rect-----------------------------------------------------------
if (rectsCollision(player.legCollisionRect, testRect))
{
	//Logi(1);
	if ((player.legCollisionRect.x + player.legCollisionRect.w - player.xVel < testRect.x) || 
		(player.legCollisionRect.x - player.xVel > testRect.x + testRect.w))
	{
		player.x = player.prevX;
		//Log("collision x");
	}
	else if ((player.legCollisionRect.y + player.legCollisionRect.h - player.yVel < testRect.y) || 
			(player.legCollisionRect.y - player.yVel > testRect.y + testRect.h))
	{
		player.y = player.prevY;
		//Log("collision y");
	}
}

//PLAYER MOVE
{
	//if (event.type == SDL_KEYDOWN)
	//{
	//	Logi(event.key.repeat);
	//}
    if (event.type == SDL_KEYDOWN && event.key.repeat == 0)
    {
        if (event.key.keysym.sym == SDLK_a)
        {   
            xVel -= speed;
        }
        else if (event.key.keysym.sym == SDLK_d)
        {
            xVel += speed;
        }
		else if (event.key.keysym.sym == SDLK_w)
		{
			yVel -= speed;
		}
		else if (event.key.keysym.sym == SDLK_s)
		{
			yVel += speed;
		}
		else if (event.key.keysym.sym == SDLK_q && isArmed)
		{
			changeGun();
			return 1;
		}
		else if (event.key.keysym.sym == SDLK_f)
		{
			yVel = 0;
			xVel = 0;
		}
		else if (event.key.keysym.sym == SDLK_SPACE && isArmed)
		{
			return 2;
		}
    }
    else if (event.type == SDL_KEYUP)
    {
        if(event.key.keysym.sym == SDLK_a)
        {
            xVel += speed;
        }
        else if (event.key.keysym.sym == SDLK_d)
        {
            xVel -= speed;
        }
		else if (event.key.keysym.sym == SDLK_w)
		{
			yVel += speed; 
		}
		else if (event.key.keysym.sym == SDLK_s)
		{
			yVel -= speed;
		}
    }
	
    if (xVel > 0) {isRight = true;}  else if (xVel < 0) {isRight = false;}
	prevX = x;
	prevY = y;
    x += xVel;
	y += yVel;	
	collisionRect = {x, y, width, height};
	legCollisionRect = {x, y + height - 6, width, 6};
	return 0;
}

//AUDIO my
struct Sound{Uint32 l; Uint8 *b;};

Sound music, shot, chest, hp, gunS
SDL_AudioSpec wavSpec;
Uint32 queueSize;
Uint8 *madeSoundB, *savedSoundB;
SDL_AudioDeviceID auDev;

void Game::addSound(Uint8 *buffer, Uint32 length)
{
	queueSize = SDL_GetQueuedAudioSize(auDev);
	if (queueSize == 0)
	{
		//*music.b = savedSound;
		for (int i = 0; i < music.l; i++) 
		{
			music.b[i] = savedSoundB[i];
		}
		SDL_QueueAudio(auDev, music.b, music.l);
		queueSize = SDL_GetQueuedAudioSize(auDev);
	}
	madeSoundB = music.b + music.l - queueSize;
	SDL_ClearQueuedAudio(auDev);
	     
	if (queueSize < length) 
	{
		for(int j = 0; j < queueSize; j++)
		{
			madeSoundB[j] += buffer[j];
		}
	}
	else
	{
		for(int j = 0; j < length; j++)
		{
			madeSoundB[j] += buffer[j];
		}
	}
	SDL_QueueAudio(auDev, madeSoundB, queueSize);
}
//load
	SDL_LoadWAV("assets/shot.wav", &wavSpec, &shot.b, &shot.l);
	SDL_LoadWAV("assets/sfbf.wav", &wavSpec, &music.b, &music.l);
	SDL_LoadWAV("assets/sfbf.wav", &wavSpec, &savedSoundB, &music.l);
	SDL_LoadWAV("assets/chest.wav", &wavSpec, &chest.b, &chest.l);
	SDL_LoadWAV("assets/gethp.wav", &wavSpec, &hp.b, &hp.l);
	SDL_LoadWAV("assets/getgun.wav", &wavSpec, &gunS.b, &gunS.l);
	
	auDev = SDL_OpenAudioDevice(0, 0, &wavSpec, 0, 0);
	SDL_PauseAudioDevice(auDev, 0);

//quit
	SDL_CloseAudioDevice(auDev);
	SDL_FreeWAV(shot.b);
	SDL_FreeWAV(music.b);
	SDL_FreeWAV(savedSoundB);
	SDL_FreeWAV(chest.b);