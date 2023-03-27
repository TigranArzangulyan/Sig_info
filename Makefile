
all: Sig_info

signal-echo: Sig_info.o
  g++ -o Sig_info Sig_info.o

signal-echo.o: Sig_info.cpp
  g++ -c Sig_info.cpp  

clean:
  rm -f Sig_info.o Sig_info
