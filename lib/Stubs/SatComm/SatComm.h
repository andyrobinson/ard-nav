#ifndef SatCommStub_h
#define SatCommStub_h

class SatComm {
    public:
      SatComm();

      virtual void begin();
      bool steer_log_or_continue();
      void set_dest(char *label);

      // for test
      void set_result(bool res);
      char dest[3];
      int steer_count;
      void reset();
      bool result;

    private:

};

#endif