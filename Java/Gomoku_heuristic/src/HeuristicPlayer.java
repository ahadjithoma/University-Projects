
import java.util.ArrayList;

public class HeuristicPlayer implements AbstractPlayer
{
  //Variables
  int score;
  int id;
  String name;
  static int pid=0;

  //Constructors
  public HeuristicPlayer (Integer pid)
  {
    id = pid;
    score = 0;
  }
 
  public HeuristicPlayer(Integer pid, int pscore, String pname){
	  score = pscore;
	  id = pid;
	  name=pname;	  
  }

  //Getters
  public String getName ()
  {
    return "Heuristic";
  }

  public int getId ()
  {
    return id;
  }

  //Setters
  public void setScore (int score)
  {
    this.score = score;
  }

  public int getScore ()
  {
    return score;
  }

  public void setId (int id)
  {
    this.id = id;
  }

  public void setName (String name)
  { 
    this.name = name;
  }
  
  
  //Calc Function: checks for possible Quintuple, Quadruple, Triple and Double creation
  boolean calc(int a,int b,int c,int d,int limit,Board board) {
	  int sum1=0;					//Tiles counter for Player1 
	  int sum2=0;					//Tiles counter for Player2
	  
	  //Tiles counter for all possible HORIZONTAL and VERTICAL sequences for both players
	  if ((a-b)==0 || (c-d)==0) {	
		  for (int i=a; i<=b; i++) {
			  for (int j=c; j<=d; j++) {
				  if (board.getTile(i,j).getColor()==1)
					  sum1++;
				  if (board.getTile(i,j).getColor()==2)
					  sum2++;
			  }
		  }
	  }
	  //Tiles counter for all possible MAIN DIAGONAL sequences for both players
	  else if(b-a<0) {				
		  for (int i=a; i>=b; i--) {
			  for (int j=c; j<=d; j++) {
				  if (i+j==a+c) {
					  if (board.getTile(i,j).getColor()==1) {
						  sum1++;
						  break;
					  }
					  if (board.getTile(i,j).getColor()==2) {
						  sum2++;
						  break;
					  }
				  }
			  }
		  }
	  }
	  //Tiles counter for all possible SECONDARY DIAGONAL sequences	for both players 
	  else {						
		  for (int i=a; i<=b; i++) {
			  for (int j=c; j<=d; j++) {
				  if (Math.abs(i-j)==Math.abs(a-c)) {
					  if (board.getTile(i,j).getColor()==1) {
						  sum1++;
						  break;
					  }
					  if (board.getTile(i,j).getColor()==2) {
						  sum2++;
						  break;
					  }
				  }
			  }
		  }
	  }
	  
	  //if sum1 or sum2 = limit then save the player1 or player2 id to pid variable
	  if (sum1==limit) 
		  pid=1;
	  if (sum2==limit)
		  pid=2;
	  
	  //if sum1 or sum2 = limit then player1 or player2 formed a specific sequence
	  if (sum1==limit || sum2==limit)
		  return true;
	  else
		  return false;
  }//end of calc
  
  
  //check Function: checks for all possible sequences based on board size
  boolean check(int x,int y,int limit,Board board) {
	  int a=x-limit;		//left board limit
	  int b=x+limit;		//right board limit
	  int c=y-limit;		//top board limit
	  int d=y+limit;		//bottom board limit 
	  int counterA=limit;	//possible sequences counter based on 'a' limit
	  int counterB=limit;	//possible sequences counter based on 'b' limit
	  int counterC=limit;	//possible sequences counter based on 'c' limit
	  int counterD=limit;	//possible sequences counter based on 'd' limit
	  int minX,minY,minXY;	//total vertical,horizontal and diagonal possible sequences
	  int x1,x2,y1,y2;		//(x1,y1): main diagonal starting possition
	  						//(x2,y2): secondary diagonal starting possition
	  
	  //setting the limits & counters for a specific position based on the board size
	  while(a<0) {
		  a++;
		  counterA--;
	  }
	  while(b>(GomokuUtilities.NUMBER_OF_ROWS-1)) {
		  b--;
		  counterB--;
	  }
	  while(c<0) {
		  c++;
		  counterC--;
	  }
	  while(d>(GomokuUtilities.NUMBER_OF_COLUMNS-1)) {
		  d--;
		  counterD--;
	  }
	  
	  //setting the total horizontal possible sequences
	  minX=counterA;
	  if (counterB<minX)
		  minX=counterB;
	  
	  //setting the total vertical possible sequences
	  minY=counterC;
	  if (counterD<minY)
		  minY=counterD;
	  
	  //checks for possible horizontal sequence using calc function
	  for (int i=a; i<=a+minX; i++)
		  if (calc(i,i+limit,y,y,limit,board)==true)
			  return true;
	  
	  //checks for possible vertical sequence using calc function
	  for (int j=c; j<=c+minY; j++)
		  if (calc(x,x,j,j+limit,limit,board)==true)
			  return true;
	  
	  //setting the starting position for main diagonal possible sequences
	  minXY=minX;
	  if (minY<minXY)
		  minXY=minY;
	  
	  if (y<(GomokuUtilities.NUMBER_OF_COLUMNS-1)/2 && y<x) {
		  x1=x-minY;
		  y1=y-minY;
	  }
	  else if(x<(GomokuUtilities.NUMBER_OF_ROWS-1)/2 && x<y) {
		  x1=x-minX;
		  y1=y-minX;
	  }
	  else if(x==y && counterA!=limit) {
		  x1=x-minX;
		  y1=y-minX;
	  }
	  else {
		  x1=x-limit;
		  y1=y-limit;
	  }
	  
	  //checks for possible main diagonal sequence using calc function
	  for (int k=0; k<=minXY; k++) {
		  if ((x1+limit)<GomokuUtilities.NUMBER_OF_ROWS && (y1+limit)<GomokuUtilities.NUMBER_OF_COLUMNS)
			  if (calc(x1,x1+limit,y1,y1+limit,limit,board)==true)
				  return true;
		  x1++;
		  y1++;
	  }
	  
	  //setting the starting position for secondary diagonal possible sequences
	  if (y<(GomokuUtilities.NUMBER_OF_COLUMNS-1)/2 && (x+y)<(GomokuUtilities.NUMBER_OF_ROWS-1)) {
		  x2=x+minY;
		  y2=y-minY;
	  }
	  else if(x>(GomokuUtilities.NUMBER_OF_ROWS-1)/2 && (x+y)>(GomokuUtilities.NUMBER_OF_COLUMNS-1)) {
		  x2=x+minX;
		  y2=y-minX;
	  }
	  else if((x+y)==(GomokuUtilities.NUMBER_OF_ROWS-1) && counterB!=limit) {
		  x2=x+minX;
		  y2=y-minY;
	  }
	  else {
		  x2=x+limit;
		  y2=y-limit;
	  }
	  
	  //checks for possible secondary diagonal sequence using calc function
	  for (int m=0; m<=minXY; m++) {
		  if ((x2-limit)>=0 && (y2+limit)<GomokuUtilities.NUMBER_OF_COLUMNS)
			  if (calc(x2,x2-limit,y2,y2+limit,limit,board)==true)
				  return true;
		  x2--;
		  y2++;
	  }
	  
	  return false;	//returns false if none of the above return true
  }//end of check
  
  
  //createsQuintuple Function: checks for possible quintuple creation using check function
  boolean createsQuintuple(int x,int y,Board board) {
	  if (check(x,y,4,board)==true)
		  return true;
	  else
		  return false;  
  }//end of createsQuintuple
  
  //creatsQuadruple Function: checks for possible quadruple creation using check function
  boolean createsQuadruple(int x,int y,Board board) {
	  if (check(x,y,3,board)==true)
		  return true;
	  else
		  return false;
  }//end of createsQuadruple
  
  //createsTriple Function: checks for possible triple creation using check function
  boolean createsTriple(int x,int y,Board board) {
	  if (check(x,y,2,board)==true)
		  return true;
	  else
		  return false;
  }//end of createsTriple 
  
  //createsDouble Function: checks for possible double creation using check function
  boolean createsDouble(int x,int y,Board board) {
	  if (check(x,y,1,board)==true)
		  return true;
	  else 
		  return false;
  }//end of createsDouble
  
  
  //centrality Function: checks the centrality of a position
  float centrality(int x,int y) {
	  int xc=(GomokuUtilities.NUMBER_OF_ROWS-1)/2;		//x central
	  int yc=(GomokuUtilities.NUMBER_OF_COLUMNS-1)/2;	//y central
	  
	  //centrality check
	  if ((x-xc)==0 && (y-yc)==0)
		  return 1;
	  else if(x==0 || y==0 || x==(GomokuUtilities.NUMBER_OF_ROWS-1) || y==(GomokuUtilities.NUMBER_OF_COLUMNS-1))
		  return 0;
	  else if(Math.abs(x-xc)<=1 && Math.abs(y-yc)<=1)
		  return (float)9/10;
	  else if(Math.abs(x-xc)<=2 && Math.abs(y-yc)<=2)
		  return (float)8/10;
	  else if(Math.abs(x-xc)<=4 && Math.abs(y-yc)<=4)
		  return (float)5/10;
	  else if(Math.abs(x-xc)<=6 && Math.abs(y-yc)<=6)
		  return (float)2/10;
	  else
		  return (float)1/10;
  }//end of centrality
  
 
  //getNextMove Function
  public int[] getNextMove (Board board)
  {
	  int place,max;
	  ArrayList<int[]> options=new ArrayList<int[]>();
	  int[] nextMove=new int[2];
	  
	  for (int x=0; x<GomokuUtilities.NUMBER_OF_ROWS; x++) {
		  for (int y=0; y<GomokuUtilities.NUMBER_OF_COLUMNS; y++) {
			  if (board.getTile(x,y).getColor()==0) {	//checks for empty tile
				  int[] array=new int[3];
				  array[0]=x;
				  array[1]=y;
				  array[2]=evaluate(x,y,board);			//saves the evaluation of (x,y) position
				  options.add(array);					
			  }
		  }
	  }

	  max=options.get(0)[2];
	  place=0;
	  //saves the position(place) with the max evaluation number  
	  for (int i=1; i<options.size(); i++) {
		  if (options.get(i)[2]>max) {
			  max=options.get(i)[2];
			  place=i;
		  }
	  }

	  nextMove[0]=options.get(place)[0];	//x of the next move
	  nextMove[1]=options.get(place)[1];	//y of the next move
			  
	  return nextMove;
  }//end of getNextMove
  
  //evaluate Function
  int evaluate (int x,int y,Board board)
  {
	  int a,b,c,d;	// a,b,c = radius 2,3,4 evaluation numbers respectively
	  				// d = centrality evaluation number
	  
	  //Color Percentage check at (x,y) position for Radius=2
	  if (GomokuUtilities.colorPercentage(board,x,y,2,id)<=0.2)
		  a=0;
	  else if(GomokuUtilities.colorPercentage(board,x,y,2,id)<=0.4)
		  a=1;
	  else if(GomokuUtilities.colorPercentage(board,x,y,2,id)<=0.6)
		  a=2;
	  else if(GomokuUtilities.colorPercentage(board,x,y,2,id)<=0.8)
		  a=3;
	  else
		  a=4;
	  
	  //Color Percentage check at (x,y) position for Radius=3
	  if (GomokuUtilities.colorPercentage(board,x,y,3,id)<=(float)10/49)
		  b=0;
	  else if(GomokuUtilities.colorPercentage(board,x,y,3,id)<=(float)20/49)
		  b=1;
	  else if(GomokuUtilities.colorPercentage(board,x,y,3,id)<=(float)30/49)
		  b=2;
	  else if(GomokuUtilities.colorPercentage(board,x,y,3,id)<=(float)40/49)
		  b=3;
	  else
		  b=4;
	  
	  //Color Percentage check at (x,y) position for Radius=4
	  if (GomokuUtilities.colorPercentage(board,x,y,4,id)<=(float)15/81)
		  c=0;
	  else if(GomokuUtilities.colorPercentage(board,x,4,2,id)<=(float)30/81)
		  c=1;
	  else if(GomokuUtilities.colorPercentage(board,x,4,2,id)<=(float)45/81)
		  c=2;
	  else if(GomokuUtilities.colorPercentage(board,x,4,2,id)<=(float)60/81)
		  c=3;
	  else
		  c=4;
	  
	  //centrality check
	  if (centrality(x,y)==1)
		  d=4;
	  else if(centrality(x,y)==0)
		  d=0;
	  else if(centrality(x,y)==(float)9/10)
		  d=3;
	  else if(centrality(x,y)==(float)8/10)
		  d=2;
	  else
		  d=1;
	  
	  //Quinuple check
	  if (createsQuintuple(x,y,board)==true) {
		  if (pid==id)
			  return 100;
		  else
			  return 99;
	  }
	  //Quadruple check
	  if (createsQuadruple(x,y,board)==true) {
		  if (pid==id) 			//checks player's id
			  return 90+c+d;	//returns total evaluation number for our player
		  else
			  return 80+c+d;	//returns total evaluation number for opposite player
	  }
	  //Triple check
	  if (createsTriple(x,y,board)==true) {
		  if (pid==id)
			  return 70+b+d;
		  else
			  return 60+b+d;
	  }
	  //Double check
	  if (createsDouble(x,y,board)==true) {
		  if (pid==id)
			  return 50+a+d;
		  else
			  return 40+a+d;
	  }
	  
	  return d;					//returns evaluation number			
  }//end of evaluate
}//end of HeuristicPlayer class
