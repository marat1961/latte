/* ----------------------------------------------------------------- */
/*                                                                   */
/* LattE (Lattice Point Enumeration)                                 */
/*                                                                   */
/* Master program                                                    */
/*  -- simplified version (mkoeppe, margulies) --                    */
/*                                                                   */
/* Author     : Raymond Hemmecke, Ruriko Yoshida                     */
/*                                                                   */
/* Created    : 07-JUN-02                                            */
/* Last Update: 03-Mar-03                                            */
/*                                                                   */
/* ----------------------------------------------------------------- */

#include <string.h>
#include <stdio.h>
#include <NTL/mat_ZZ.h>
#include <NTL/vec_ZZ.h>
#include <NTL/mat_RR.h>
#include <NTL/vec_RR.h>
#include <NTL/config.h>
#include <NTL/LLL.h>
#include <NTL/HNF.h>
#include <NTL/ZZ.h>

#include "myheader.h"
#include "barvinok/dec.h"
#include "barvinok/barvinok.h"
#include "barvinok/Cone.h"
#include "barvinok/ConeDecom.h"
#include "barvinok/Triangulation.h"
#include "vertices/cdd.h"
#include "genFunction/maple.h"
#include "genFunction/piped.h"
#include "cone.h"
#include "ConeDeterminant.h"
#include "dual.h"
#include "RudyResNTL.h"
#include "Grobner.h"
//  #include "jesus.h"
#include "preprocess.h"
#include "print.h"
#include "ramon.h"
#include "rational.h"
#include "timing.h"
#include "flags.h"
//#include "testing.h"
#include "IntegralHull.h"
#include "ReadingFile.h"
#include "binarySearchIP.h"
#include "CheckEmpty.h" 

#include "banner.h"

/* ----------------------------------------------------------------- */
int main(int argc, char *argv[]) {
#ifdef SUN
  struct tms tms_buf;
#endif
  float z;
  int i,numOfVars,numOfAllVars, degree = 1;
  unsigned int flags = 0, print_flag = 0, output_cone = 0;
  vector dim, v, w;
  int oldnumofvars;
  vector *generators;
  char fileName[127], invocation[127], decompose[10], equationsPresent[10],
    assumeUnimodularCones[127], dualApproach[127], taylor[127], printfile[127],
    rationalCone[127], nonneg[127], Memory_Save[127], Save_Tri[127],
    Load_Tri[127], Print[127], inthull[127], cddstyle[127], grobner[127],
    removeFiles[127], command[127], maximum[127],  Singlecone[127], LRS[127],
    Vrepresentation[127], dilation[127], minimize[127], binary[127], interior[127];
  listVector *matrix, *equations, *inequalities, *rays, *endRays, *tmpRays, *matrixTmp;
  vector cost;
  listVector *templistVec;
  listCone *cones, *tmp, *tmpcones;

  latte_banner(cout);

  z=0;
  setbuf(stdout,0);

  strcpy(invocation,"Invocation: ");
  strcat(invocation,argv[0]);
  strcat(invocation," ");
/*    strcat(invocation,argv[argc-1]); */
/*    strcat(invocation,"\n\n"); */
/*    printf(invocation); */

  strcpy(Vrepresentation,"no");
  strcpy(interior,"no");
  strcpy(dilation,"no");
  strcpy(binary,"no");
  strcpy(Singlecone,"no");
  strcpy(removeFiles,"yes");
  strcpy(grobner,"no");
  strcpy(maximum,"no");
  strcpy(minimize,"no");
  strcpy(decompose,"yes");
  strcpy(dualApproach,"no");
  strcpy(equationsPresent,"no");
  strcpy(assumeUnimodularCones,"no");
  strcpy(printfile,"no");
  strcpy(taylor,"no");
  strcpy(rationalCone,"no");
  strcpy(nonneg, "no");
  strcpy(Memory_Save, "yes");
  strcpy(Save_Tri, "no");
  strcpy(Load_Tri, "no");
  strcpy(Print, "no");
  strcpy(inthull, "no");
  strcpy(cddstyle, "no");
  strcpy(LRS, "no");

  for (i=1; i<argc-1; i++) {
    strcat(invocation,argv[i]);
    strcat(invocation," ");
    if (strncmp(argv[i],"vrep",3)==0) strcpy(Vrepresentation,"yes"); 
    // if (strncmp(argv[i],"bbs",3)==0) strcpy(binary,"yes");
    if (strncmp(argv[i],"int",3)==0) strcpy(interior,"yes");
    //if (strncmp(argv[i],"min",3)==0) strcpy(minimize,"yes");
    //if (strncmp(argv[i],"gro",3)==0) strcpy(grobner,"yes");
    //if (strncmp(argv[i],"nodecom",3)==0) strcpy(decompose,"no");
    if (strncmp(argv[i],"homog",3)==0) {strcpy(dualApproach,"yes"); flags |= DUAL_APPROACH;}
    if (strncmp(argv[i],"equ",3)==0) strcpy(equationsPresent,"yes");
    if (strncmp(argv[i],"uni",3)==0) strcpy(assumeUnimodularCones,"yes");
    //if (strncmp(argv[i],"simp",4)==0) {strcpy(printfile,"yes"); flags |= PRINT;}
    if(strncmp(argv[i],"file",4)==0) strcpy(Memory_Save, "no");
    //if (strncmp(argv[i],"single",6)==0) strcpy(Singlecone,"yes");
    //if (strncmp(argv[i],"tay",3)==0) strcpy(taylor,"yes");
    //if (strncmp(argv[i],"ehrhartsimp",3)==0) strcpy(rationalCone,"yes");
    if (strncmp(argv[i],"+", 1) ==0) strcpy(nonneg,"yes");
    if (strncmp(argv[i],"memsave",7)==0) strcpy (Memory_Save, "yes");
    if (strncmp(argv[i],"printcones",3)==0) strcpy (Print, "yes");
    if (strncmp(argv[i],"cdd",3)==0) strcpy (cddstyle, "yes");
    //if (strncmp(argv[i],"hull",3)==0) strcpy (inthull, "yes");
    // if (strncmp(argv[i],"max",3)==0) strcpy (maximum, "yes");
    if (strncmp(argv[i],"lrs",3)==0) strcpy (LRS, "yes");
    if (strncmp(argv[i],"dil",3)==0) strcpy (dilation, "yes");
    if (strncmp(argv[i],"rem",3)==0) strcpy (removeFiles, "no");
    if (strncmp(argv[i],"rem",3)==0) strcpy (Memory_Save, "no");
    if (strncmp(argv[i],"trisave",7)==0) {strcpy (Save_Tri, "yes"); flags |= SAVE;}
    if (strncmp(argv[i],"triload",7)==0) {strcpy (Load_Tri, "yes"); flags |= LOAD;}
  }
  if(minimize[0] == 'y') strcpy(maximum, "yes");
  if(grobner[0] == 'y') strcpy(equationsPresent,"yes");
  if(binary[0] == 'y') {strcpy(maximum,"yes"); strcpy(Memory_Save, "no");}
  if(maximum[0] == 'y') strcpy(Memory_Save, "no");
  if(printfile[0] == 'y') strcpy(Memory_Save, "no");
  if(rationalCone[0] == 'y') strcpy(Memory_Save, "no");
  if(printfile[0] == 'y') print_flag = 1;
  if(taylor[0] == 'y'){
    degree = atoi(argv[argc-2]);
  }
  if(rationalCone[0] == 'y'){
    
    //HugInt digit(argv[1]);
    //conv(output_cone, digit.BigInt);
    // User can use only Mode one
    output_cone = 3;
  }
  int dilation_const = 1;

  if(dilation[0] == 'y') dilation_const = atoi(argv[argc-2]);

  if(output_cone > 3) output_cone = 0;
  flags |= (output_cone << 1);
  if((cddstyle[0] == 'y') && (Vrepresentation[0] == 'y')){
    cerr << "Use not cdd style and v-representation." << endl;
    exit(2);
  }
  if((dualApproach[0] == 'y') && (nonneg[0] == 'y')&&(equationsPresent[0] == 'n')){
    cerr<<"You cannot use + and dua at the same time." << endl;
    exit(2);
  }
  
  if((Memory_Save[0] == 'y') && (inthull[0] == 'y')){
    cerr<<"You cannot use int and memsave at the same time." << endl;
    exit(3);
  }

  strcat(invocation,argv[argc-1]);
  strcat(invocation,"\n\n");
  cout << invocation;
  char costFile[127];
  if(maximum[0] == 'y'){
    strcpy(fileName,argv[argc-1]);
    strcpy(costFile, argv[argc - 1]);
    strcat(costFile, ".cost");
  }
  else strcpy(fileName,argv[argc-1]);
  //  cout << fileName << " " << costFile << endl;
  if(maximum[0] == 'y') {
    ifstream ReadTest(fileName);
    if(!ReadTest){
      cerr << "Need a polytope input file." << endl;
      exit(2);
    }
    //    cout << fileName << " " << costFile << endl;
    ifstream INCost(costFile);
    if(!INCost){
      cerr << "Need a cost input file." << endl;
      exit(3);
    }
    int costDim, dummy;
    INCost >> dummy >> costDim; 
    cost.SetLength(costDim);
    for(i = 0; i < costDim; i++)
      INCost >> cost[i]; 
  }
  //strcpy (fileName,"stdin");
  
  /* Check input file. */
  if(Vrepresentation[0] == 'n'){
    if((cddstyle[0] == 'n') && (grobner[0] == 'n') && (maximum[0] == 'n')&& (minimize[0] == 'n')){
      CheckInputFile(fileName);
      CheckLength(fileName, equationsPresent);
    }
    if(minimize[0] == 'y')  strcpy (maximum, "yes");   
 
    if((cddstyle[0] == 'n') && (grobner[0] == 'n') && (maximum[0] == 'y')){
      CheckInputFile(fileName);
      CheckLength(fileName,equationsPresent);
    }

    if(cddstyle[0] == 'y')
      { CheckInputFileCDDRep(argv[argc - 1]);
      CheckInputFileCDDRep1(argv[argc - 1]);
      CheckInputFileCDDRep3(argv[argc - 1]);
      CheckInputFileCDDRep4(argv[argc - 1]);
      }
  }else CheckInputFileVrep(fileName);
  CheckEmpty(fileName);
  //vector cost;
  /* Read problem data. */
  if((cddstyle[0] == 'n') && (Vrepresentation[0] == 'n')) CheckRed(fileName, equationsPresent, maximum, nonneg, interior, dilation, dilation_const); 

  dilation_const = 1;
  if((cddstyle[0] == 'n') && (grobner[0] == 'n'))
    readLatteProblem(fileName,&equations,&inequalities,equationsPresent,
		     &numOfVars, nonneg, dualApproach, grobner, maximum, 
		     cost,Vrepresentation);
//   if((equationsPresent[0] == 'n') && (interior[0] == 'y'))
//     Interior(inequalities);
  // if(minimize[0] == 'y') cost = -cost;
  if(cddstyle[0] == 'y'){
    int tmpoutput, tmpflags;
    CDDstylereadLatteProblem(fileName,&equations,&inequalities,equationsPresent,
			     &numOfVars, nonneg, dualApproach, taylor, degree,
			     rationalCone, tmpoutput, tmpflags, Memory_Save,
			     assumeUnimodularCones, inthull, grobner);
    output_cone = 3;
    flags = tmpflags;
  }
  
  // cout << grobner << endl;
  vector holdCost;
  if(minimize[0] == 'y') cost = - cost;
  holdCost = cost;
  //cout <<"Cost is: " << cost << endl;
  vec_RR holdcost_RR;
  holdcost_RR.SetLength(holdCost.length());
  for(i = 0; i < holdCost.length(); i++) conv(holdcost_RR[i], holdCost[i]);

  if(minimize[0] == 'y') holdcost_RR = - holdcost_RR;
  if(grobner[0] == 'y'){

    CheckGrobner(fileName, cddstyle);
    SolveGrobner(fileName,  nonneg, dualApproach,
		 grobner, equationsPresent, cddstyle);}
  else{
  if((dualApproach[0] == 'y') && (nonneg[0] == 'y')&&(equationsPresent[0] == 'n')){
    cerr<<"You cannot use + and dua at the same time." << endl;
    exit(2);
  }
  
  if((Memory_Save[0] == 'y') && (inthull[0] == 'y')){
    cerr<<"You cannot use int and memsave at the same time." << endl;
    exit(3);
  }
  
  if((Vrepresentation[0] == 'y') && (equationsPresent[0] == 'y')){
    cerr<<"You cannot use vrep and equ at the same time." << endl;
    exit(4);
  }

  numOfVars--;
  /* Binary seach IP*/

  if(binary[0] == 'y'){
    cout << "The number of optimal solutions: " << binarySearch(equations, inequalities,cost, numOfVars, minimize) << endl;
    cout << "Time: " << GetTime() << endl;
    exit(0);
  }

  numOfAllVars=numOfVars;
  mat_ZZ ProjU, ProjU2, AA;
  vec_ZZ bb;
  mat_ZZ AAA;

  ProjU.SetDims(numOfVars, numOfVars);
  ProjU2.SetDims(numOfVars, numOfVars);
  oldnumofvars = numOfVars;
  generators=createArrayVector(numOfVars);
  if (equationsPresent[0]=='y') {
    /*    if(grobner[0] == 'y')
	  {  
     matrixTmp=Grobner(equations,inequalities,&generators,&numOfVars, &templistVec, oldnumofvars);
     
     }*/
    matrixTmp=preprocessProblem(equations,inequalities,&generators,&numOfVars, cost, ProjU, interior, dilation_const);
    ProjU2 = transpose(ProjU);
    bb = ProjU2[0];
    AAA.SetDims(ProjU2.NumRows() - 1, ProjU2.NumCols());
    for(i = 1; i <= numOfVars; i++){
      AAA[i - 1] = ProjU2[i];
    }
    AA = transpose(AAA);
    // cout << ProjU << determinant(transpose(AAA)*AAA) <<  endl;
      templistVec = transformArrayBigVectorToListVector(ProjU, ProjU.NumCols(), ProjU.NumRows()); 
  } else {
    dilateListVector(inequalities, numOfVars, dilation_const);
    matrixTmp=inequalities;
  }
  if((dualApproach[0] == 'y')&&(equationsPresent[0]=='y')){
   matrix = TransformToDualCone(matrixTmp,numOfVars);
   }
  else {
    matrix = matrixTmp;}
/* Now matrix contains the new inequalities. */
  RR LP_OPT;
    cout << "\nTime: " << GetTime() << " sec\n\n";
    //   cout << "Project down cost function: " << cost << endl;
    vec_RR Rat_solution, tmp_den, tmp_num;
    mat_RR ProjU_RR;
    ProjU_RR.SetDims(ProjU.NumRows(), ProjU.NumCols());
    for(i = 0; i < ProjU.NumRows(); i++)
      for(int j = 0; j < ProjU.NumCols(); j++) conv(ProjU_RR[i][j], ProjU[i][j]);
    //cout << ProjU << ProjU_RR << endl;
    Rat_solution.SetLength(numOfVars);
    tmp_den.SetLength(numOfVars);
    tmp_num.SetLength(numOfVars);
/* Compute vertices and edges. */
    rationalVector* LP_vertex;
    if ((dualApproach[0]=='n') && (Vrepresentation[0] == 'n')) {
    if(LRS[0] == 'n')
    tmpcones=computeVertexCones(fileName,matrix,numOfVars);
    else
      tmpcones=computeVertexConesViaLrs(fileName,matrix,numOfVars);
    {cones = tmpcones;
    cout << "\nThe polytope has " << lengthListCone(cones) << " vertices.\n";
    //system("rm numOfLatticePoints");
    cout << endl;}
  } 

    /* Compute triangulation or decomposition of each vertex cone. */

    {

      if (assumeUnimodularCones[0]=='n') {


      cones=dualizeCones(cones,numOfVars);
      
      if (decompose[0]=='y') 
      {
	if(Memory_Save[0] == 'n')      
     	 	cones=decomposeCones(cones,numOfVars, flags, fileName);
	// Iterator through simplicial cones, DFS
	else
		decomposeCones_Single(cones,numOfVars, degree, flags, fileName);	
      }
      
      if(Memory_Save[0] == 'n')
      	cones=dualizeBackCones(cones,numOfVars);
    }
   }

/* Compute points in parallelepipeds, unless we already did using memsave version!  */


 if(Memory_Save[0] == 'n')
 {
	 cout << "Computing the points in the Parallelepiped of the unimodular Cones." << endl;
  	tmp=cones;
	int	Cones_Processed_Count = 0;
 	while (tmp) 
	{
    		if (decompose[0]=='n') 
		{
      			tmp->latticePoints=pointsInParallelepiped(tmp->vertex,tmp->rays,0,numOfVars);
    		} 
		else 
		{
      			tmp->latticePoints=pointsInParallelepipedOfUnimodularCone(tmp->vertex,tmp->rays,numOfVars);
    		}
		
    		tmp=tmp->rest;

		Cones_Processed_Count++;

		if ((Cones_Processed_Count % 1000) == 0 )
			cout << Cones_Processed_Count << " cones processed." << endl;
  	}
}
  



 if(Print[0] == 'y')
  printListCone(cones,numOfVars);

 {
   if(Memory_Save[0] == 'n')
     {

       if(dualApproach[0] == 'n'){
	 cout << "Creating generating function.\n"; 
	 //printListVector(templistVec, oldnumofvars); cout << ProjU << endl;
	 if(equationsPresent[0] == 'y'){ cones = ProjectUp2(cones, oldnumofvars, numOfVars, AA, bb);
	 numOfVars = oldnumofvars;}

	 createGeneratingFunctionAsMapleInput(fileName,cones,numOfVars);
       }
       //printListCone(cones, numOfVars);
       if(dualApproach[0] == 'n'){
	 cout << "Starting final computation.\n";
	 cout << endl << "****  The number of lattice points is: " << Residue(cones,numOfVars) << "  ****" << endl << endl;
       }

}
 }

 if(rationalCone[0] == 'y') {
   strcpy(command, "mv ");
   strcat(command, "simplify.sum ");
   strcat(command, argv[argc - 1]);
   strcat(command, ".rat");
   system(command);
 }

 if(printfile[0] == 'y'){
   strcpy(command, "mv ");
   strcat(command, "func.rat ");
   strcat(command, argv[argc - 1]);
   strcat(command, ".rat");
   system(command);
 }
 if((removeFiles[0] == 'y') && (dualApproach[0] == 'n')){
   
  strcpy(command,"rm ");
  strcat(command,fileName);
  strcat(command,".ext");
  system(command);
  
  strcpy(command,"rm ");
  strcat(command,fileName);
  strcat(command,".cdd");
  system(command); 
  
  if(Memory_Save[0] == 'n'){
    strcpy(command,"rm ");
    strcat(command,fileName);
    strcat(command,".maple");
    system(command); 
  }

  strcpy(command,"rm ");
  strcat(command,fileName);
  strcat(command,".ead");
  system(command); 
  
  if(cddstyle[0] == 'n'){
    strcpy(command,"rm ");
    strcat(command,fileName);
    system(command);
  }
 }
  }


  if((dualApproach[0] == 'y') && (cddstyle[0] == 'n')){

    strcpy(command,"rm ");
    strcat(command,fileName);
    system(command);

  }
 cout << "Computation done. " << endl;
 cout << "Time: " << GetTime() << " sec\n\n";
 
 return(0);
}
/* ----------------------------------------------------------------- */


