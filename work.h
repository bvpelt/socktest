
class Work
{
private:
    int connection;
    int status; // 0 started, 1 active, 2 ended
    int retval;

public:
    Work();
    Work(const int connection, const int status, const int retval);
    ~Work();

    int getConnection();
    void setConnection(const int connection);
    int getStatus();
    void setStatus(const int status);
    int getRetval();
    void setRetval(const int retval);
};
