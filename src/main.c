#include <stdint.h>
#include "bsp/bsp.h"
#include "lib_draw.h"
#include "imagenes/fondo1.h"
#include "imagenes/bala2.h"
#include "imagenes/mario_izq0.h"
#include "imagenes/mario_izq1.h"
#include "imagenes/mario_der0.h"
#include "imagenes/mario_der1.h"
#include "imagenes/mario_par_der0.h"
#include "imagenes/mario_par_der1.h"
#include "imagenes/mario_par_izq0.h"
#include "imagenes/mario_par_izq1.h"
#include "imagenes/mario_sal_der2.h"
#include "imagenes/mario_sal_der3.h"
#include "imagenes/mario_sal_izq0.h"
#include "imagenes/mario_sal_izq1.h"
#include "imagenes/mario_muerto.h"
#include "imagenes/enemigo_muerto.h"
#include "imagenes/game_over.h"
#include "imagenes/game_winer.h"
#include "imagenes/inicio.h"
#include "imagenes/fondo4.h"
#include "imagenes/fondo5.h"
#include "imagenes/fondo6.h"
#include "imagenes/enemi.h"
#include "imagenes/enemi2.h"

uint16_t posx = 42, posy = 34, posx_sal, posy_sal, posx_enemigo = 600,
		posy_enemigo = 37, posx_enemigo_muerto, posy_enemigo_muerto,
		contador_muerte = 0;
int posx_fondo_nube = 320, posy_fondo_nube = 150, posx_fondo1 = 400,
		posy_fondo1 = 37, posx_inicio = 119, posy_inicio = 70, posx_dead = 96,
		posy_dead = 40,enemigos_dead=0;


uint32_t mario = 0;
uint8_t SW_up = 0;
uint8_t SW_down = 0;
uint8_t SW_right = 0;
uint8_t SW_left = 0;
uint8_t direccion_right = 1;
uint8_t direccion_left = 0;
uint8_t direccion_up = 1;
uint8_t direccion_down = 1;
uint8_t direccion_enemigo_right = 1;
uint8_t direccion_enemigo_left = 1;
uint8_t state = 8, actualizar_state = 1, state_active, contador = 0,
		mario_dead = 0, enemigo1 = 1, fondo0 = 1, enemigo2 = 1, enemigo_muerto =
				0, flag_inicio = 1;

void State();
void Swich();
void Muerte();

enum {
	run_derecha = 0,
	run_izquierda = 1,
	salto = 2,
	salto_derecha = 3,
	salto_izquierda = 4,
	abajo_izquierda = 5,
	parado_izquierda = 6,
	parado_derecha = 7,
	abajo_derecha = 8
};

int main(void) {

	bsp_init();

	while (1) {
		if (contador == 0) {
			Swich();
		}
		VGA_DrawImage(&fondo1, 0, 0);

		if (fondo0) {
			VGA_DrawImageAlpha(&fondo4, posx_fondo1, posy_fondo1, 0x73);

			if (posx_fondo1 < -400) {
				posx_fondo1 = 350;
			}

			VGA_DrawImageAlpha(&fondo_nube, posx_fondo_nube, posy_fondo_nube,
					0x73);
			if (posx_fondo_nube < -150) {
				posx_fondo_nube = 320;
			}

			VGA_DrawImageAlpha(&fondo5, posx_fondo1 + 300, posy_fondo1, 0x73);

		}

		if (flag_inicio) {
			if (state == run_derecha) {
				if (posx_inicio > -200) {
					posx_inicio = posx_inicio - 3;
				} else {
					flag_inicio = 0;
				}

			}
			VGA_DrawImageAlpha(&inicio, posx_inicio, posy_inicio, 0x73);

		}

		if (enemigo1) {
			if(enemigos_dead <= -3 || enemigos_dead >=3){
				enemigo1=0;
			}
			if (enemigo_muerto) {
				contador_muerte++;
				if (contador_muerte < 50) {
					VGA_DrawImageAlpha(&enemi_muerto, posx_enemigo_muerto,
							posy_enemigo_muerto, 0x73);
				} else {
					enemigo_muerto = 0;
					contador_muerte = 0;
				}
			}

			if (mario++ % 8 > 3) {
				VGA_DrawImageAlpha(&enemi1, posx_enemigo, posy_enemigo, 0x73);
			} else {
				VGA_DrawImageAlpha(&enemi2, posx_enemigo, posy_enemigo, 0x73);
			}

			if (direccion_enemigo_left) {
				direccion_enemigo_right = 0;
				if (posx_enemigo > 3) {
					posx_enemigo = posx_enemigo - 2;
				} else {
					direccion_enemigo_right = 1;
					direccion_enemigo_left = 0;
				}
			}
			if (direccion_enemigo_right) {
				direccion_enemigo_left = 0;
				if (posx_enemigo < 300) {
					posx_enemigo = posx_enemigo + 2;
				} else {
					direccion_enemigo_right = 0;
					direccion_enemigo_left = 1;
				}
			}

		}else{
			if (enemigos_dead > 1){
				VGA_DrawImageAlpha(&game_winer, 85,20, 0x00);
			}else{
				VGA_DrawImageAlpha(&game_over, 85,20, 0x00);
			}




		}


		Muerte();

		if (mario_dead == 1) {
			VGA_DrawImageAlpha(&mario_muerto, posx_dead, posy_dead, 0x73);

			if (contador > 10) {
				direccion_down = 0;
				direccion_up = 1;
				contador--;
				posy_dead = posy_dead+1;

				if (mario++ % 8 > 3) {
					VGA_DrawImageAlpha(&mario_muerto, posx_dead, posy_dead,
							0x73);
				} else {
					VGA_DrawImageAlpha(&mario_muerto, posx_dead, posy_dead,
							0x73);
				}

			} else {
				if (contador > 0) {
					direccion_down = 1;
					direccion_up = 0;
					contador--;
					posy_dead = posy_dead - 4;

					if (mario++ % 8 > 3) {
						VGA_DrawImageAlpha(&mario_muerto, posx_dead, posy_dead,
								0x73);
					} else {
						VGA_DrawImageAlpha(&mario_muerto, posx_dead, posy_dead,
								0x73);
					}

				} else {
					mario_dead = 0;
				}

			}
		} else {
			if(enemigos_dead>=-2){
				State();
			}

		}

		bsp_draw();

	}
}

void Swich() {
	if (!get_sw_state(SW_RIGHT)) {
		//
		direccion_left = 0;
		direccion_right = 1;
		actualizar_state = 1;
		state = run_derecha;
		SW_right = 1;
		if (posx < 300) {
			posx = posx + 5;
		}
	}
	if (!get_sw_state(SW_LEFT)) {
		direccion_left = 1;
		direccion_right = 0;
		actualizar_state = 1;
		state = run_izquierda;
		SW_left = 1;
		if (posx > 5) {
			posx = posx - 5;
		}
	}


	if (!get_sw_state(SW_RIGHT) && !get_sw_state(SW_UP)) {

		if (SW_up == 0) {
			SW_up = 1;
			direccion_left = 0;
			direccion_right = 1;
			state = salto_derecha;
			contador = 10;
		}
	}
	if (!get_sw_state(SW_LEFT) && !get_sw_state(SW_UP)) {
		if (SW_up == 0) {
			SW_up = 1;
			direccion_left = 1;
			direccion_right = 0;
			state = salto_izquierda;
			contador = 10;
		}
	}

	if (!get_sw_state(SW_UP)) {
		if (SW_up == 0) {
			actualizar_state = 1;
			state = salto;
			contador = 10;
			SW_up = 1;
		}
	} else {
		SW_up = 0;
	}

	/*	if (!get_sw_state(SW_DOWN)) {
	 actualizar_state = 1;
	 state = abajo_izquierda;
	 //contador = 10;
	 SW_down = 1;
	 }*/
	if (contador == 0 && SW_up == 1 && get_sw_state(SW_DOWN)
			&& get_sw_state(SW_RIGHT) && get_sw_state(SW_LEFT)) {
		if (direccion_left && direccion_right == 0) {
			state = parado_izquierda;
		} else {
			state = parado_derecha;
		}

	}
	if (!get_sw_state(SW_LEFT) && !get_sw_state(SW_UP)
			&& !get_sw_state(SW_RIGHT) && !get_sw_state(SW_UP)) {
		if (direccion_left && direccion_right == 0) {
			state = parado_izquierda;
		} else {
			state = parado_derecha;
		}

	}
	if (get_sw_state(SW_LEFT) && get_sw_state(SW_UP) && get_sw_state(SW_RIGHT)
			&& get_sw_state(SW_UP)) {
		if (direccion_left && direccion_right == 0) {
			state = parado_izquierda;
		} else {
			state = parado_derecha;
		}

	}

}

void State() {
	actualizar_state = 0;
	switch (state) {
	case run_derecha:
		posx_fondo_nube = posx_fondo_nube - 10;
		posx_fondo1 = posx_fondo1 - 10;
		if (mario++ % 8 > 3) {
			VGA_DrawImageAlpha(&mario_der0, posx, posy, 0x53);
		} else {
			VGA_DrawImageAlpha(&mario_der1, posx, posy, 0x53);
		}
		break;
	case run_izquierda:
		posx_fondo_nube = posx_fondo_nube + 5;
		posx_fondo1 = posx_fondo1 + 5;
		if (mario++ % 8 > 3) {
			VGA_DrawImageAlpha(&mario_izq0, posx, posy, 0x53);
		} else {
			VGA_DrawImageAlpha(&mario_izq1, posx, posy, 0x53);
		}
		break;
	case salto:
		if (contador > 5) {
			direccion_down = 0;
			direccion_up = 1;
			contador--;
			posy = posy + 6;
			if (direccion_left) {
				if (mario++ % 8 > 3) {
					VGA_DrawImageAlpha(&mario_sal_izq0, posx, posy, 0x53);
				} else {
					VGA_DrawImageAlpha(&mario_sal_izq1, posx, posy, 0x53);
				}
			} else {
				if (mario++ % 8 > 3) {
					VGA_DrawImageAlpha(&mario_sal_der2, posx, posy, 0x53);
				} else {
					VGA_DrawImageAlpha(&mario_sal_der3, posx, posy, 0x53);
				}

			}

		} else {
			if (contador > 0) {
				direccion_down = 1;
				direccion_up = 0;
				contador--;
				posy = posy - 6;
				if (direccion_left) {
					if (mario++ % 8 > 3) {
						VGA_DrawImageAlpha(&mario_sal_izq0, posx, posy, 0x53);
					} else {
						VGA_DrawImageAlpha(&mario_sal_izq1, posx, posy, 0x53);
					}
				} else {
					if (mario++ % 8 > 3) {
						VGA_DrawImageAlpha(&mario_sal_der2, posx, posy, 0x53);
					} else {
						VGA_DrawImageAlpha(&mario_sal_der3, posx, posy, 0x53);
					}

				}
			}

		}

		break;
	case salto_derecha:
		posx_fondo_nube = posx_fondo_nube - 10;
		posx_fondo1 = posx_fondo1 - 10;
		if (contador > 5) {
			direccion_down = 0;
			direccion_up = 1;
			contador--;
			if (posx < 300) {
				posx = posx + 4;
			}
			posy = posy + 6;
			if (mario++ % 8 > 3) {
				VGA_DrawImageAlpha(&mario_sal_der2, posx, posy, 0x53);
			} else {
				VGA_DrawImageAlpha(&mario_sal_der3, posx, posy, 0x53);
			}
		} else {
			if (contador > 0) {
				direccion_down = 1;
				direccion_up = 0;
				contador--;
				if (posx < 300) {
					posx = posx + 4;
				}
				posy = posy - 6;
				if (mario++ % 8 > 3) {
					VGA_DrawImageAlpha(&mario_sal_der2, posx, posy, 0x53);
				} else {
					VGA_DrawImageAlpha(&mario_sal_der3, posx, posy, 0x53);
				}

			}
		}

		break;
	case salto_izquierda:
		posx_fondo_nube = posx_fondo_nube + 5;
		posx_fondo1 = posx_fondo1 + 5;

		if (contador > 5) {
			direccion_down = 0;
			direccion_up = 1;
			contador--;

			if (posx > 10) {
				posx = posx - 4;
			}
			posy = posy + 6;
			if (mario++ % 8 > 3) {
				VGA_DrawImageAlpha(&mario_sal_izq0, posx, posy, 0x53);
			} else {
				VGA_DrawImageAlpha(&mario_sal_izq1, posx, posy, 0x53);
			}
		} else {
			if (contador > 0) {
				direccion_down = 1;
				direccion_up = 0;
				contador--;
				if (posx > 10) {
					posx = posx - 4;
				}
				posy = posy - 6;
				if (mario++ % 8 > 3) {
					VGA_DrawImageAlpha(&mario_sal_izq0, posx, posy, 0x53);
				} else {
					VGA_DrawImageAlpha(&mario_sal_izq1, posx, posy, 0x53);
				}
			}

		}

		break;
	case abajo_izquierda:
		if (mario++ % 8 > 3) {
			VGA_DrawImageAlpha(&mario_sal_izq0, posx, posy, 0x53);
		} else {
			VGA_DrawImageAlpha(&mario_sal_izq1, posx, posy, 0x53);
		}
		break;
	case parado_izquierda:
		if (mario++ % 8 > 3) {
			VGA_DrawImageAlpha(&mario_par_izq0, posx, posy, 0x53);
		} else {
			VGA_DrawImageAlpha(&mario_par_izq1, posx, posy, 0x53);
		}
		break;
	case parado_derecha:
		if (mario++ % 8 > 3) {
			VGA_DrawImageAlpha(&mario_par_der0, posx, posy, 0x53);
		} else {
			VGA_DrawImageAlpha(&mario_par_der1, posx, posy, 0x53);
		}
		break;
	case abajo_derecha:
		posx_fondo_nube = posx_fondo_nube - 10;
		posx_fondo1 = posx_fondo1 - 10;
		if (mario++ % 8 > 3) {
			VGA_DrawImageAlpha(&mario_sal_izq0, posx, posy, 0x53);
		} else {
			VGA_DrawImageAlpha(&mario_sal_izq1, posx, posy, 0x53);
		}
		break;
	default:
		break;
	}

}

void Muerte() {

	if ((posx + 15) > (posx_enemigo + 5)) {
		if ((posx + 15) < posx_enemigo + 20) {
			if (posy + 3 > posy_enemigo) {
				if ((posy - posy_enemigo) < 9) {
					if (direccion_up == 0) {
						enemigos_dead++;
						enemigo_muerto = 1;
						posy_enemigo_muerto = 37;
						posx_enemigo_muerto = posx_enemigo;
						posx_enemigo = 500;
					} else {
						// murio mario
						enemigos_dead--;
						contador = 20;
						mario_dead = 1;
						posx_dead = posx;
						posy_dead = posy;
						posx = 5;
						posy = 34 ;
						posx_enemigo = 500;
					}

				}
			} else {
				//murio mario
				enemigos_dead--;
				contador = 20;
				mario_dead = 1;
				posx_dead = posx;
				posy_dead = posy;
				posx = 5;
				posy = 34 ;
				posx_enemigo = 500;

			}
		}

	}

}

