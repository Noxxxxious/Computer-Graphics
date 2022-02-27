
#include <math.h>
#include <iostream>
#include <stdio.h>
#define V 7 //liczba wierzcholkow gwiazdy
#define R 50 //promien kola w ktore gwiazda jest wpisana
// Do��cz definicje biblioteki Allegro
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

const float FPS = 60;		//obraz b�dzie aktualizowany co 1/FPS sekundy
const int SCREEN_W = 640;	//szeroko�� okna
const int SCREEN_H = 480;	//wysoko�� okna

void star(int n, int r, float x, float y, float fi) {
	ALLEGRO_COLOR white = al_map_rgb(255, 255, 255);
	float dx[V * 2];
	float dy[V * 2];
	float Vertices[4 * V];
	float a = ALLEGRO_PI / n;
	for (int i = 0; i < 2 * n; i++) {
		float angle = a * i;
		dx[i] = r * cos(angle) / (1 + i % 2);
		dy[i] = r * sin(angle) / (1 + i % 2);
	}
	for (int i = 0; i < 2 * n; i++)
	{
		Vertices[2 * i] = (dx[i] * cos(fi) - dy[i] * sin(fi) + 0.5) + x;
		Vertices[2 * i + 1] = (dx[i] * sin(fi) + dy[i] * cos(fi) + 0.5) + y;
	}
	al_draw_polygon(Vertices, 2 * n, 1, white, 3, 1);
}
// Funkcja g��wna
int main()
{
	ALLEGRO_DISPLAY *display = NULL;			//okno
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;	//kolejka zdarzen
	ALLEGRO_TIMER *timer = NULL;				//timer, od ktorego b�dziemy odbierac zdarzenia (potrzebny do animacji)
	bool redraw = true;

	if (!al_init()) {							//inicjalizacja biblioteki Allegro
		fprintf(stderr, "Nie zainicjalizowano allegro!\n");
		return -1;
	}
	
	display = al_create_display(SCREEN_W, SCREEN_H);	//utworznie okna
	timer = al_create_timer(1.0 / FPS);					//utworzenie timera
	al_install_keyboard();								//inicjalizacja obs�ugi klawiatury
	event_queue = al_create_event_queue();				//utworzenie kolejki zdarze�
	
	al_init_primitives_addon();							//inicjalizacja obs�ugi prostych element�w (punkty, linie, prostok�ty, elipsy itd.)

	//Rejestracja �r�de� zdarze� (okno, timer, klawiatura ...)
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	
	//Kolory rysowania
	ALLEGRO_COLOR yellow = al_map_rgb(255, 255, 0);
	ALLEGRO_COLOR white = al_map_rgb(255, 255, 255);
	ALLEGRO_COLOR blue = al_map_rgb(0, 0, 255);
	ALLEGRO_COLOR black = al_map_rgb(0, 0, 0);

	//Definicja wielok�ta
	const int N = 7;
	float dx[N] = { 0.0, -30.0, -10.0, -10.0, 10.0, 10.0, 30.0 };
	float dy[N] = { -60.0, -30.0,-30.0, 60.0, 60.0,-30.0,-30.0 };
	

	//Tablice na przetworzone wsp�rz�dna punkt�w
	float points[2*N];

	//Zmienne na potrzeby obracania figury
	double fi=0.0, dfi=0.1, sinfi, cosfi;

	//Uruchamiamy timer, kt�ry b�dzie z zadan� cz�stotliwo�ci� wysy�a� zdarzenia
	al_start_timer(timer);
	
	//P�tla g��wna programu - obs�uga zdarze�.
	//Dzia�amy, dop�ki u�ytkownik nie wci�nie Esc.
	float offset_x = 0, offset_y = 0, vel_x = 2, vel_y = 2;
	while(true)
	{
		ALLEGRO_EVENT event;
		al_wait_for_event(event_queue, &event);

		if (event.type == ALLEGRO_EVENT_TIMER) {	//zdarzenie timera -> od�wie�enie obrazu 
			redraw = true;
		}
		else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {	//zdarzenie klawiatury -> je�li Esc to ko�czymy
			if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
			break;
		}
		else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) { //zdarzenie zamkni�cia okna
			break;
		}

		if (redraw && al_is_event_queue_empty(event_queue)) 
		{
			redraw = false;
			al_clear_to_color(black); //czyszczenie okna na zadany kolor

			//Wyznacz �rodek ekranu
			int xm = SCREEN_W / 2;
			int ym = SCREEN_H / 2;
			//Obr�t figury
			sinfi = sin(fi);
			cosfi = cos(fi);
			for (int i = 0; i < N; i++)
			{
				points[2 * i] = (dx[i] * cosfi - dy[i] * sinfi + 0.5) + xm;
				points[2 * i + 1] = (dx[i] * sinfi + dy[i] * cosfi + 0.5) + ym;
			}
			fi += dfi;

			//Narysuj wype�niony okr�g
			//al_draw_filled_circle(xm, ym, 100, blue);
			//al_draw_circle(xm, ym, 100, yellow, 2);

			//Narysuj wype�niony wielokat
			//al_draw_filled_polygon(points, N, white);
			if (offset_x >= xm - R || offset_x <= -xm + R)
				vel_x = -vel_x;
			if (offset_y >= ym - R || offset_y <= -ym + R)
				vel_y = -vel_y;
			offset_x += vel_x;
			offset_y += vel_y;
			star(V, R, xm + offset_x, ym + offset_y, fi);
			//Wy�wietl w oknie to, co narysowano w buforze
			al_flip_display();
		}
	}

	al_destroy_display(display);
	al_destroy_timer(timer);
	al_destroy_event_queue(event_queue);
	return 0;
}
