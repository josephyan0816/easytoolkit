//
// Created by Joseph Yan on 2023/1/20.
//

#ifndef EASYTOOLKIT_PIPEWRAP_H
#define EASYTOOLKIT_PIPEWRAP_H


namespace toolkit{
    class PipeWrap{
    public:
        PipeWrap();
        ~PipeWrap();
        int write(const void *buf,int n);
        int read(void *buf,int n);
        int readFD() const{
            return _pipe_fd[0];
        }
        int writeFD() const{
            return _pipe_fd[1];
        }
    private:
        void clearFD();
    private:
        int _pipe_fd[2]={-1,-1};
    };
}

#endif //EASYTOOLKIT_PIPEWRAP_H
