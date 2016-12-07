//Dimitris Niras     Andreas Hadjithoma
//8057               8026
//6972580826         6993418085
//diminira@auth.gr   antreasc@auth.gr
public class MinMaxPlayer implements AbstractPlayer
{
  //variables
  int score;
  int id;
  String name;
  int oid;
  
  //Constructor
  public MinMaxPlayer (Integer pid)
  {
    id = pid;
    score = 0;
    if(pid==1)
    	oid=2;
    else
    	oid=1;
  }
  
  //Getters & Setters
  public String getName ()
  {
    return "MinMax";
  }

  public int getId ()
  {
    return id;
  }

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

  //getNextMove Function: returns best avaiable move
  public int[] getNextMove (Board board)
  {
    int[] bestTile={0,0};	//bestTile initialization 
    Board clone=GomokuUtilities.cloneBoard(board); //board clone
    Node80268057 root=new Node80268057(clone); //root creation  
    createMySubTree(root,1); //tree creation
    bestTile=chooseMove(root); //chooseMove call 
    return bestTile;  
  }//end of getNextMove
  
  //creatMySubTree Function: creates the nodes of our available moves
  private void createMySubTree (Node80268057 parent,int depth)
  {
	  for(int x=0; x<GomokuUtilities.NUMBER_OF_ROWS; x++) {
		  for(int y=0; y<GomokuUtilities.NUMBER_OF_COLUMNS; y++) {
			  if(parent.nodeBoard.getTile(x,y).getColor()==0) {
				  Board clone=GomokuUtilities.cloneBoard(parent.nodeBoard); 
				  GomokuUtilities.playTile(clone,x,y,id); 
				  Node80268057 newNode=new Node80268057(parent,clone,x,y,id);
				  newNode.nodeEvaluation=newNode.evaluate();
				  parent.children.add(newNode);
				  createOpponentSubTree(newNode,depth+1);
			  }
		  }
	  }
  }//end of creatMySubTree

  //createOpponentSubTree Function: creates the nodes of opponent's available move
  private void createOpponentSubTree (Node80268057 parent, int depth)
  {
	  for(int x=0; x<GomokuUtilities.NUMBER_OF_ROWS; x++) {
		  for(int y=0; y<GomokuUtilities.NUMBER_OF_COLUMNS; y++) {
			  if (parent.nodeBoard.getTile(x,y).getColor()==0) {
				  Board clone=GomokuUtilities.cloneBoard(parent.nodeBoard);
				  GomokuUtilities.playTile(clone,x,y,oid);
				  Node80268057 subNewNode=new Node80268057(parent,clone,x,y,oid);
				  subNewNode.nodeEvaluation=-subNewNode.evaluate(); //stores evaluation number
				  parent.children.add(subNewNode); 
			  }
		  }
	  }
  }//end of createOpponentSubTree

  //chooseMove Function: returns next move's coordinates 
  private int[] chooseMove (Node80268057 root)
  {
	  int[] nextMove=new int[2];
	  double min,max;
	  
	  //getting the min value
	  for(int i=0; i<root.children.size(); i++) {
		  min=root.children.get(i).children.get(0).nodeEvaluation; 
		  //root.children.get(i).nodeEvaluation=min;	
		  for(int j=1; j<root.children.get(i).children.size(); j++) {
			  if(root.children.get(i).children.get(j).nodeEvaluation<min) {
				  min=root.children.get(i).children.get(j).nodeEvaluation;
				  //root.children.get(i).nodeEvaluation=min;
			  }
		  }
		  root.children.get(i).nodeEvaluation+=min;
	  }

	  //getting the max value
	  max=root.children.get(0).nodeEvaluation;
	  nextMove[0]=root.children.get(0).nodeMove[0];
	  nextMove[1]=root.children.get(0).nodeMove[1];
	  for(int i=1; i<root.children.size(); i++) {
		  if((root.children.get(i).nodeEvaluation)>max) {
			  max=root.children.get(i).nodeEvaluation;
			  nextMove[0]=root.children.get(i).nodeMove[0];
			  nextMove[1]=root.children.get(i).nodeMove[1];
		  }
	  }
	  return nextMove;
  }//end of chooseMove 
  
}//end of MinMaxPlayer
