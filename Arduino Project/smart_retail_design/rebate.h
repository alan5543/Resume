class rebate {
  private:
  byte first;
  byte second;
  byte third;
  byte forth;
  int point;
  
  public:
  rebate(){
    first = 0;
    second = 0;
    third = 0;
    forth = 0;
    point = 0;
  }
  rebate(byte first, byte second, byte third, byte forth, int point){
    this->first = first;
    this->second = second;
    this->third = third;
    this->forth = forth;
    this->point = point;
  }
  void change_1(byte f){
    first = f;
  }
  void reduce_point(){
    this->point = 0; //used all the rebate point
  }
  void add_point(int x){
    this->point = this->point + x;
  }
  byte firstbyte(){
    return first;
  }
  byte secondbyte(){
    return second;
  }
  byte thirdbyte(){
    return third;
  }
  byte forthbyte(){
    return forth;
  }
  int getPoint(){
    return point;
  }
};
