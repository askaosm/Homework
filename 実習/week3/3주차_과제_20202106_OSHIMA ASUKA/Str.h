
class Str{

  private:
     char* str;  
     int len; // the length of string
  public:
     Str(int leng); //leng is the length of string
     Str(const char* neyong); //initializ
     ~Str(); //destrucotr
     int length(void);  
     char* contents(void);
     int compare(class Str &a); // strcmp
     int compare(const char *a); //strcmp
     void operator=( const char *a);
     void operator=( class Str& a);

};


