#include <fcntl.h>
#include <unistd.h>
#include <string>

typedef unsigned int uint;

int k = 1;

std::string hend(const char * fileName, bool _end, bool _stdin = false)
{
  std::string fn(fileName);
  if (_stdin) fn = "Standard Input";
  std::string hend = "";
  hend += "\n\n";
  for (uint i = 0; i < fn.length() + 10; i++)
    hend += "-";
  hend += "\n";
  if (_end) hend += "End of ";
  hend += fn + "\n";
  for (uint i = 0; i < fn.length() + 10; i++)
    hend += "-";
  hend += "\n\n";
  return hend;
}

std::string randCharP(const int & len)
{
  std::string s = "";
  s += '.';
  char letters[] = "qwertyuiopasdfghjklzxcvbnm";
  for (int i = 1; i < len-1; i++)
    {
      s += letters[rand() % (len-1)];
      srand(++k);
    }
  s += '\0';
  return s;
}

int workingFD(std::string & s)
{
  int _fd, _nb;
  _fd = open(s.c_str(), O_RDONLY);
  if (s.compare("-") == 0)
    {
      return STDIN_FILENO;
    }
  while (_fd == -1)
    {
      std::string a = "---------------------------------------\n";
      a += "That file doesn't exist. Want to try another one?\n";
      a += "--------------------------------------------\n\n";
      write(STDOUT_FILENO, a.c_str(), a.length() + 1);
      int fd1 = open(randCharP(25).c_str(), O_RDWR | O_TRUNC | O_CREAT, S_IRWXO | S_IRWXG | S_IRWXU);
      lseek(fd1, 0, SEEK_SET);
      char buf[1];
      int i = 0;
      while ((_nb = read(STDIN_FILENO, buf, 1)) > 0)
	write(fd1, buf, 1);
      i = lseek(fd1, 0, SEEK_END);
      char * chars = new char[i+2];
      pread(fd1, chars, i, 0);
      chars[i] = '\0';
      std::string _s(chars);
      s = _s;
      close(fd1);
      _fd = open(chars, O_RDONLY);
    }
  return _fd;
}

void handleOutput(char * arg)
{
  int fd, nb;
  const char * _c = arg;
  std::string s(_c);
  fd = workingFD(s);
  _c = s.c_str();
  nb = lseek(fd, 0, SEEK_END);
  char * buf = new char[nb + 1];
  lseek(fd, 0, SEEK_SET);
  nb = read(fd, buf, nb);
  buf[nb] = '\0';
  close(fd);
  write(STDOUT_FILENO, hend(_c, false).c_str(), hend(_c, false).length() + 1);
  write(STDOUT_FILENO, buf, nb+1);
  write(STDOUT_FILENO, hend(_c, true).c_str(), hend(_c, true).length() + 1);
  delete[] buf;
}

void handleStdIn()
{
  char buf[1];
  int i = 0;
  std::string prompt = hend("Please enter into Standard Input.", false);
  write(STDOUT_FILENO, prompt.c_str(), prompt.length() + 1);
  int fd1 = open(randCharP(25).c_str(), O_RDWR | O_TRUNC | O_CREAT);
  lseek(fd1, 0, SEEK_SET);
  int nb;
  while ((nb = read(STDIN_FILENO, buf, 1)) > 0)
    write(fd1, buf, 1);
  i = lseek(fd1, 0, SEEK_END);
  char * chars = new char[i+2];
  pread(fd1, chars, i, 0);
  chars[i] = '\0';
  const char a = '-'; const char *b = &a;
  write(STDOUT_FILENO, hend(b, false, true).c_str(), hend(b, false, true).length() + 1);
  write(STDOUT_FILENO, chars, i+1);
  write(STDOUT_FILENO, hend(b, true, true).c_str(), hend(b, true, true).length() + 1);
  delete[] chars;
  close(fd1);
}

int main(int argc, char * argv[])
{
  if (argc == 1) handleStdIn();
  for (int i = 1; i < argc; i++)
    {
      std::string s(argv[i]);
      if (s.compare("-") == 0)
	handleStdIn();
      else
	handleOutput(argv[i]);
    }
}
