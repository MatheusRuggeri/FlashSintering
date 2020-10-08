void(* resetFunc) (void) = 0; //declare reset function @ address 0

void reset(){
  delay(10);
  resetFunc();  //call reset
}
