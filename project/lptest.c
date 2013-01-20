#include "lp_lib.h"

int main(){
	lprec *lp;
	int Ncol, *colno=NULL, j, ret=0;
	REAL *row=NULL;

	Ncol=2;
	lp=make_lp(0,Ncol);

	if(lp==NULL){
		return 1;
	}

	set_col_name(lp,1,"x1");
	set_col_name(lp,2,"x2");

	colno=(int *) malloc(Ncol*sizeof(*colno));
	row=(REAL *) malloc(Ncol*sizeof(row));
	
	if((colno==NULL)||(row==NULL))
		return 1;
	
	set_add_rowmode(lp, TRUE);
	//x1>1
	colno[0]=1;
	row[0]=1;

	colno[1]=2;
	row[1]=0;

	add_constraintex(lp,2,row,colno,GE,1);

	//x2>1
	colno[0]=2;
	row[0]=0;

	colno[1]=2;
	row[1]=1;

	add_constraintex(lp,2,row,colno,GE,1);

	//x1+x2>2
	colno[0]=1;
	row[0]=1;

	colno[1]=2;
	row[1]=1;

	add_constraintex(lp,2,row,colno,GE,2);

	set_add_rowmode(lp,FALSE);

	//min: x1+x2
	colno[0]=1;
	row[0]=1;

	colno[1]=2;
	row[1]=1;

	set_obj_fnex(lp,2,row,colno);
	set_minim(lp);
	write_LP(lp,stdout);
	solve(lp);

	printf("Objective value: %f\n",get_objective(lp));

	get_variables(lp,row);

	for(j=0;j<Ncol;j++)
		printf("%s: %f\n",get_col_name(lp,j+1),row[j]);

	free(row);
	free(colno);

	return 0;
}
