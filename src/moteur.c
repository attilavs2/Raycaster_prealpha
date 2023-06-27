#include <math.h>
#include <stdlib.h>
#include <string.h>

#include <gint/display.h>
#include <gint/keyboard.h>

#include "moteur.h"
#include "sprites.h"
#include "map_test.h"

unsigned short table_couleur[235] = {
	0x10a2,0x18a3,0x18a3,0x18c3,0x18c3,0x18c3,0x18c3,
	0x18e3,0x18e3,0x20e4,0x20e4,0x2104,0x2104,0x2104,
	0x2104,0x2124,0x2124,0x2124,0x2925,0x2945,0x2945,
	0x2945,0x2945,0x2965,0x2965,0x2965,0x3166,0x3186,
	0x3186,0x3186,0x3186,0x31a6,0x31a6,0x31a6,0x39a7,
	0x39c7,0x39c7,0x39c7,0x39c7,0x39e7,0x39e7,0x39e7,
	0x41e8,0x4208,0x4208,0x4208,0x4208,0x4228,0x4228,
	0x4228,0x4a29,0x4a49,0x4a49,0x4a49,0x4a49,0x4a69,
	0x4a69,0x4a69,0x4a69,0x528a,0x528a,0x528a,0x528a,
	0x52aa,0x52aa,0x52aa,0x52aa,0x5aab,0x5acb,0x5acb,
	0x5acb,0x5acb,0x5aeb,0x5aeb,0x5aeb,0x62ec,0x630c,
	0x630c,0x630c,0x630c,0x632c,0x632c,0x632c,0x6b2d,
	0x6b4d,0x6b4d,0x6b4d,0x6b4d,0x6b6d,0x6b6d,0x6b6d,
	0x6b6d,0x738e,0x738e,0x738e,0x738e,0x73ae,0x73ae,
	0x73ae,0x73ae,0x7bcf,0x7bcf,0x7bcf,0x7bcf,0x7bef,
	0x7bef,0x7bef,0x7bef,0x8410,0x8410,0x8410,0x8410,
	0x8430,0x8430,0x8430,0x8430,0x8c51,0x8c51,0x8c51,
	0x8c51,0x8c71,0x8c71,0x8c71,0x8c71,0x9492,0x9492,
	0x9492,0x9492,0x94b2,0x94b2,0x94b2,0x94b2,0x94d2,
	0x9cd3,0x9cd3,0x9cd3,0x9cf3,0x9cf3,0x9cf3,0x9cf3,
	0x9d13,0xa514,0xa514,0xa514,0xa534,0xa534,0xa534,
	0xa534,0xa554,0xad55,0xad55,0xad55,0xad55,0xad75,
	0xad75,0xad75,0xad75,0xb596,0xb596,0xb596,0xb596,
	0xb5b6,0xb5b6,0xb5b6,0xb5b6,0xb5d6,0xbdd7,0xbdd7,
	0xbdd7,0xbdf7,0xbdf7,0xbdf7,0xbdf7,0xbe17,0xc618,
	0xc618,0xc618,0xc638,0xc638,0xc638,0xc638,0xc658,
	0xce59,0xce59,0xce59,0xce79,0xce79,0xce79,0xce79,
	0xce99,0xd69a,0xd69a,0xd69a,0xd6ba,0xd6ba,0xd6ba,
	0xd6ba,0xd6da,0xdedb,0xdedb,0xdedb,0xdefb,0xdefb,
	0xdefb,0xdefb,0xdf1b,0xdf1b,0xe71c,0xe71c,0xe73c,
	0xe73c,0xe73c,0xe73c,0xe75c,0xe75c,0xef5d,0xef5d,
	0xef7d,0xef7d,0xef7d,0xef7d,0xef9d,0xef9d,0xf79e,
	0xf79e,0xf7be,0xf7be,0xf7be,0xf7be,0xf7de,0xf7de,
	0xffdf,0xffdf,0xffff,0xffff
};

float deg_to_rad(float angle_deg) {
	return((angle_deg * pi) / 180);
}

void compute_table() {
	extern unsigned short angle_30;//il y en a des inutilisés
	extern unsigned short angle_15;
	extern unsigned short angle_90;
	extern unsigned short angle_180;
	extern unsigned short angle_270;
	extern unsigned short angle_360;
	extern unsigned short angle_5;
	extern unsigned short angle_10;
	extern unsigned short angle_45;
	extern int cos_table[ang_360p];
	extern int sin_table[ang_360p];
	extern int tan_table[ang_360p];
	extern int acos_table[ang_360p];
	extern int asin_table[ang_360p];
	extern int atan_table[ang_360p];
	extern float tab_mur_x[ang_360p];
	extern float tab_mur_y[ang_360p];
	extern int distors_table[ang_360p];
	int i = 0;
	float rad_i = 0;
	while (i != angle_360) {
		rad_i = deg_to_rad(i);
		cos_table[i] = floor( 64 * cos(rad_i));
		sin_table[i] = floor( 64 * sin(rad_i));
		tan_table[i] = floor( 64 * tan(rad_i));
		acos_table[i] = floor( 64 * acos(rad_i));
		asin_table[i] = floor( 64 * asin(rad_i));
		atan_table[i] = floor( 64 * atan(rad_i));
		if (i >= 90 && i < angle_270) {
			tab_mur_x[i] = tile_size / tan(rad_i);
			if (tab_mur_x[i] > 0) {
				tab_mur_x[i] = -tab_mur_x[i];
			}
		}
		else {
			tab_mur_x[i] = tile_size / tan(rad_i);
			if (tab_mur_x[i] < 0) {
				tab_mur_x[i] = -tab_mur_x[i];
			}
		}
		if (i >= 0 && i < angle_180) {
			tab_mur_y[i] = tile_size / tan(rad_i);
			if (tab_mur_y[i] < 0) {
				tab_mur_y[i] = -tab_mur_y[i];
			}
		}
		else {
			tab_mur_y[i] = tile_size / tan(rad_i);
			if (tab_mur_y[i] > 0) {
				tab_mur_y[i] = -tab_mur_y[i];
			}
		}
		i++;
	}
	for (i = -30; i <= angle_30;) {
		distors_table[i + angle_30] = floor(128*(1 / cos(deg_to_rad(i))));
		i++;
	}
}
void draw_walls(){
	int vertical_grid;
	int horizontal_grid;
	int dist_to_n_v_grid; //distance to next vertical grid
	int dist_to_n_h_grid;
	int x_intersect;
	int y_intersect;
	float dist_to_n_x_intersect;
	float dist_to_n_y_intersect;
	int x_raypos;
	int y_raypos;
	int dist_to_v_hit;
	int dist_to_h_hit;
	int castarc = 0;
	int castcolumn;
	int xtemp;
	int ytemp;
	int mapindex;
	int wall_haut;
	int wall_bas;
	int half_viewport_h = viewport_h / 2; //Va y avoir besoin quand je vais 
	//réduire par deux la résolution horizontale du bouzin
	int rgh_xdist;
	int rgh_ydist; 
	char wall_type; //type de mur touché par le raycast
	int wall_dist;
	float scale_factor;
	float proj_wall_h;
	double wall_temp;
	extern unsigned short angle_30;//il y en a peut être des inutilisés, a voir
	extern unsigned short angle_15;
	extern unsigned short angle_90;
	extern unsigned short angle_180;
	extern unsigned short angle_270;
	extern unsigned short angle_360;
	extern unsigned short angle_5;
	extern unsigned short angle_10;
	extern unsigned short angle_45;
	extern int cos_table[ang_360p];
	extern int sin_table[ang_360p];
	extern int tan_table[ang_360p];
	extern int acos_table[ang_360p];
	extern int asin_table[ang_360p];
	extern int atan_table[ang_360p];
	extern float tab_mur_x[ang_360p];
	extern float tab_mur_y[ang_360p];
	extern int distors_table[ang_360p];
	extern int player_x;
	extern int player_y;
	extern int player_dir;
	extern short map_test[map_w][map_h];

	castarc = player_dir - angle_30;
	if (castarc < 1) {
		castarc += angle_360;
	}
	if (castarc > 360) {
		castarc -= angle_360;
	}
	for ( castcolumn = 0; castcolumn < viewport_w;) {

		#ifdef debug
		dprint( 1, 1, C_BLACK, "castcolumn : %d", castcolumn); 
		dprint( 1, 10, C_BLACK, "castarc : %d", castarc);
		#endif

		if (castarc > 0 && castarc < angle_180) {
			horizontal_grid = floor(player_y / tile_size) * tile_size + tile_size;
			dist_to_n_h_grid = tile_size;
			xtemp = floor((atan_table[castarc] * (horizontal_grid - player_y)) >> 6);
			x_intersect = xtemp + player_x;

			#ifdef debug
			dprint( 1, 20, C_BLACK, "x_intersect : %d", x_intersect);
			#endif
		}
		else {
			horizontal_grid = floor(player_y / tile_size) * tile_size;
			dist_to_n_h_grid = -tile_size;
			xtemp = floor((atan_table[castarc] * (horizontal_grid - player_y)) >> 6);
			x_intersect = xtemp + player_x;
			horizontal_grid--;

			#ifdef debug
			dprint( 1, 20, C_BLACK, "x_intersect : %d", x_intersect);
			#endif
		}

		if (castarc == 0 || castarc == angle_180) {
			dist_to_h_hit = max_dist;
		}
		else {
			dist_to_n_x_intersect = tab_mur_x[castarc];
			while (true) {
				x_raypos = floor(x_intersect / tile_size);
				y_raypos = floor(horizontal_grid / tile_size);
				//rgh_xdist = abs(x_raypos - (tile_size * player_x));
				//rgh_ydist = abs(y_raypos- (tile_size * player_y));
                //Gros truc bien long pour vérifier que le rayon est pas trop loin
                //Note : Si je remplace le <= par < avec les raypos, ça plante :/
				if (x_raypos >= map_w || y_raypos >= map_h || x_raypos <= 0 || y_raypos <= 0 
					|| rgh_xdist > max_dist || rgh_ydist > max_dist) {
					dist_to_h_hit = max_dist;

                    #ifdef raycast_debug
					dprint( 200, 1, C_BLACK, "H : Max_dist");
					#endif
					break;
				}
				else if (map_test[x_raypos][y_raypos] == 0) {
					x_intersect += dist_to_n_x_intersect;
					horizontal_grid += dist_to_n_h_grid;
				}
				else {
					dist_to_h_hit = floor(((x_intersect - player_x) * acos_table[castarc]) >> 6);
					wall_type = map_test[x_raypos][y_raypos];

                    #ifdef raycast_debug
					dprint( 200, 10, C_BLACK, "H : Hit");
                    dprint( 200, 20, C_BLACK, "x_intersect : %d", x_intersect);
                    dprint( 200, 30, C_BLACK, "horizontal_grid : %d", horizontal_grid);
                    dprint( 200, 40, C_BLACK, "wall_type : %d", wall_type);
					#endif
					break;
				}
			}

			#ifdef debug
			dprint( 1, 30, C_BLACK, "dist_to_h_hit : %d", dist_to_h_hit);
			#endif
		}

		if (castarc < angle_90 || castarc > angle_270) {
			vertical_grid = tile_size + floor(player_x / tile_size) * tile_size;
			dist_to_n_v_grid = tile_size;
			ytemp = floor((tan_table[castarc] * (vertical_grid - player_x)) >> 6 );
			y_intersect = ytemp + player_y;

			#ifdef debug
			dprint( 1, 40, C_BLACK, "y_intersect : %d", y_intersect);
			#endif
		}
		else {
			vertical_grid = floor(player_x / tile_size * tile_size);
			dist_to_n_v_grid = -tile_size;
			ytemp = floor((tan_table[castarc] * (vertical_grid - player_x)) >> 6 );
			y_intersect = ytemp + player_y;
			vertical_grid--;

			#ifdef debug
			dprint( 1, 40, C_BLACK, "y_intersect : %d", y_intersect);
			#endif
		}

		if (castarc == angle_90 || castarc == angle_270) {
			dist_to_v_hit = max_dist;
		}
		else {
			dist_to_n_y_intersect = tab_mur_y[castarc];
			while (true) {
				x_raypos = floor(vertical_grid / tile_size);
				y_raypos = floor(y_intersect / tile_size);
				//rgh_xdist = abs(x_raypos - (tile_size * player_x));
				//rgh_ydist = abs(y_raypos - (tile_size * player_y));
				if (x_raypos >= map_w || y_raypos >= map_h || x_raypos <= 0 || y_raypos <= 0 
					|| rgh_xdist > max_dist || rgh_ydist > max_dist) {
					dist_to_h_hit = max_dist;

					#ifdef raycast_debug
					dprint( 200, 50, C_BLACK, "V : Max_dist");
					#endif
					break;
				}
				else if (map_test[x_raypos][y_raypos] == 0) {
					x_intersect += dist_to_n_x_intersect;
					horizontal_grid += dist_to_n_h_grid;
                }
				else {
					dist_to_h_hit = floor(((x_intersect - player_x) * acos_table[castarc]) >> 6);
					wall_type = map_test[x_raypos][y_raypos];

					#ifdef raycast_debug
					dprint( 200, 60, C_BLACK, "V : Hit");
                    dprint( 200, 70, C_BLACK, "x_intersect : %d", x_intersect);
                    dprint( 200, 80, C_BLACK, "horizontal_grid : %d", horizontal_grid);
                    dprint( 200, 90, C_BLACK, "wall_type : %d", wall_type);
					#endif
					break;
				}
			}

			#ifdef debug
			dprint( 1, 50, C_BLACK, "dist_to_v_hit : %d", dist_to_v_hit);
			#endif
		}
		if (dist_to_h_hit <= dist_to_v_hit) {
			wall_dist = dist_to_h_hit;
		}
		else {
			wall_dist = dist_to_v_hit;
		}
		//wall_temp = floor(wall_dist / distors_table[castcolumn]) >> 7; //va y avoir de la 
		//distortion mais pas grave

		#ifdef debug
		dprint( 1, 60, C_BLACK, "wall_dist : %d", wall_dist);
		#endif

		proj_wall_h = floor((tile_size * player_pj_pl_dist / wall_dist)*0.5);
		wall_bas = half_viewport_h + proj_wall_h;
		wall_haut = half_viewport_h + proj_wall_h;
		if (wall_haut < 0) {
			wall_haut = 0;
		}
		if (wall_bas >= viewport_h) {
			wall_bas = viewport_h - 1;
		}
		
		wall_bas = (wall_bas - wall_haut) + 1;

		/*if (floor(wall_dist) < max_dist) {
			couleur = floor(255 - (wall_dist / max_dist) * 255)-20;
			if (couleur <= 0) {
				couleur = 0;
			}
			if (couleur > 235) {
				couleur = 235;
			}
			switch (wall_type) {
				case 1: {
					drect(castcolumn, wall_haut, castcolumn, wall_bas, wall_color);
					break;
				}
				case 2: {
					drect(castcolumn, wall_haut, castcolumn, wall_bas, 0xFAFA);
					break;
				}
			}
			drect( castcolumn, wall_haut, castcolumn, wall_bas, 0xAAAA); //nrmlnt : table_couleur[couleur]	
		}
		else {
			drect( castcolumn, wall_haut, castcolumn, wall_bas, 0x5ACB);
		}
		*/
		dupdate();
		getkey();
		dclear(C_WHITE);
		
		castarc++;
		castcolumn++;
		if (castarc > angle_360) {
			castarc -= angle_360;
		}
	}
}