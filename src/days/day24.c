#include "../common.h"
#include <stdbool.h>
#include <assert.h>
#include <time.h>

#define NUMHAIL 300
#define EPS 1e-10
#define DIM 6

typedef struct Point{
    double x;
    double y; 
    double z;
}Point;

typedef struct Hail{
    long long data[6];
}Hail;

typedef struct State{
    Hail hail[NUMHAIL];
}State;

void handleLine(char* line, int row, void* params){
    State* s = (State*) params;
    int dummy = 0;
    get_numbers_ll(line, s->hail[row].data, &dummy);
}

Point collisionPoint2d(Hail h1, Hail h2){

    double x1 = h1.data[0], y1 = h1.data[1];
    double x2 = x1 + h1.data[3], y2 = y1 + h1.data[4];

    double x3 = h2.data[0], y3 = h2.data[1];
    double x4 = x3 + h2.data[3], y4 = y3 + h2.data[4];

    Point p;
    double D = ((double)(x1 - x2)*(y3 - y4) - (double)(y1 - y2)*(x3 - x4));
    p.x = (double) ((x1*y2 - y1*x2)*(x3 - x4) - (x1 - x2)*(x3*y4 - y3*x4)) / (double) D;
    p.y = (double) ((x1*y2 - y1*x2)*(y3 - y4) - (y1 - y2)*(x3*y4 - y3*x4)) / (double) D;
    return p;
}

bool inFuture(Hail h, Point p){
    double xdiff = p.x - h.data[0], ydiff = p.y - h.data[1];
    return sign(xdiff) == sign(h.data[3]) && sign(ydiff) == sign(h.data[4]);
}

bool withinArea(Point p, long long min, long long max){
    return (p.x + EPS) >= min && (p.x - EPS) <= max && (p.y + EPS) >= min && (p.y - EPS) <= max;
}

int score1(State* s){

    int sum = 0;
    long long min = 2e14, max = 4e14;
    for(int i = 0; i < NUMHAIL; i++){
        for(int j = i+1; j < NUMHAIL; j++){
            Point p = collisionPoint2d(s->hail[i], s->hail[j]);
            if(fpclassify(p.x) == FP_INFINITE || fpclassify(p.y) == FP_INFINITE){
                continue;
            }
            if(withinArea(p, min, max) && inFuture(s->hail[i], p) && inFuture(s->hail[j], p)){
                sum++;
            }
        }
    }
    return sum;
}

bool condition(double a, double b){
    return fabs(a - b) < EPS*(fabs(a) + fabs(b));
}

void swap(double *a, double *b){
    double dummy = *a; 
    *a = *b; 
    *b = dummy; 
}

bool GE(double A[DIM][DIM], double b[DIM], double* x){

    //Get to RRE FORM
    for(int i = 0; i < DIM; i++){

        int index = 0;
        double max = 0;
        for(int j = i; j < DIM; j++){
            if(fabs(A[i][j]) > max){
                max = fabs(A[i][j]);
                index = j;
            }
        }

        if(condition(max, 0.0)){
            return false;
        }

        swap(&b[i], &b[index]);
		for(int j = 0; j < DIM; j++){
            swap(&A[i][j], &A[index][j]);
        }
			
		for(int k = i+1; k < DIM; k++){
			double r = A[k][i] / A[i][i];
			for(int l = i; l < DIM; l++){
                A[k][l] -= r*A[i][l];
            }
			b[k] -= r*b[i];
		}
    }

    // Substitute
	for(int i = DIM-1; i >= 0; i--){
		x[i] = b[i];
		for(int j = i+1; j < DIM; j++){
            x[i] -= A[i][j]*x[j];
        }
		x[i] /= A[i][i];
	}
	return true;
}

//Solve the linear system using GE. System setup heavily inspired by someone else.
//Could not get the first setup i used to work.
//Equations are on the form:
//(v_y1-v_y2)x - (v_x1-v_x2)y + - (y_1-y_2)v_x + (x_1-x_2)v_y = y_2*v_x2 + x_2*v_y2 - y_1*x_1 - x_1*v_y1
//Produces different results for different triples of hails due to numerical instability.
//It can probably be improved on, but ended up doing some trial and error.
double score2(State* s){

    Hail h0 = s->hail[3], h1 = s->hail[4], h2 = s->hail[5];

    double A[DIM][DIM] = {
        //0, 1, x, y
        {h0.data[4] - h1.data[4], h1.data[3] - h0.data[3], 0,
            h1.data[1] - h0.data[1], h0.data[0] - h1.data[0], 0},
        //0, 2, x, y
        {h0.data[4] - h2.data[4], h2.data[3] - h0.data[3], 0,
            h2.data[1] - h0.data[1], h0.data[0] - h2.data[0], 0},
        //0, 1, x, z
        {h0.data[5] - h1.data[5], 0,  h1.data[3] - h0.data[3],
            h1.data[2] - h0.data[2], 0, h0.data[0] - h1.data[0]},
        //0, 2, x, z
        {h0.data[5] - h2.data[5], 0,  h2.data[3] - h0.data[3],
            h2.data[2] - h0.data[2], 0, h0.data[0] - h2.data[0]},
        //0, 1, y, z
        {0, h0.data[5] - h1.data[5], h1.data[4] - h0.data[4],
            0, h1.data[2] - h0.data[2], h0.data[1] - h1.data[1]},
        //0, 2, y, z
        {0, h0.data[5] - h2.data[5], h2.data[4] - h0.data[4],
            0, h2.data[2] - h0.data[2], h0.data[1] - h2.data[1]},
    };
    //Same ordering as above
    double b[DIM] = {
        h1.data[1]*h1.data[3] - h1.data[0]*h1.data[4] - 
            h0.data[1]*h0.data[3] + h0.data[0]*h0.data[4],
        h2.data[1]*h2.data[3] - h2.data[0]*h2.data[4] -
            h0.data[1]*h0.data[3] + h0.data[0]*h0.data[4],
        h1.data[2]*h1.data[3] - h1.data[0]*h1.data[5] - 
            h0.data[2]*h0.data[3] + h0.data[0]*h0.data[5],
        h2.data[2]*h2.data[3] - h2.data[0]*h2.data[5] - 
            h0.data[2]*h0.data[3] + h0.data[0]*h0.data[5],
        h1.data[2]*h1.data[4] - h1.data[1]*h1.data[5] - 
            h0.data[2]*h0.data[4] + h0.data[1]*h0.data[5],
        h2.data[2]*h2.data[4] - h2.data[1]*h2.data[5] - 
            h0.data[2]*h0.data[4] + h0.data[1]*h0.data[5],
    };
    //Solution vector
    double x[DIM];
    if(!GE(A, b, x)){
        printf("Failure!\n");
        return 0;
 
    }
    Point p = {.x = x[0], .y = x[1], .z = x[2]};
    return round(p.x + p.y + p.z);
}

int main(int argc, char *argv[]){
  
    State *s = calloc(1, sizeof(State));
    srand(time(NULL));
    assert(s);
    char* fname = "data/day24.txt";
    get_lines(fname, handleLine, s);
    printf("Score1: %d\n", score1(s));
    printf("Score2: %.2lf\n", score2(s));
    free(s);
    return 0;
}