template<typename T>
T llceil(T a, T b){
  if(a%b==0){return a/b;}
 
  if(a>=0){return (a/b)+1;}
  else{return -((-a)/b);}
}

template<typename T> 
T llfloor(T a, T b){
  if(a%b==0){return a/b;}
 
  if(a>=0){return (a/b);}
  else{return -((-a)/b)-1;}
}