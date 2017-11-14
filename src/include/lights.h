#ifndef SRC_INCLUDE_LIGHTS_H_
#define SRC_INCLUDE_LIGHTS_H_

class Light {
    public:
        Light() {}
        Light(vec3 dir, vec3 a, vec3 d, vec3 s) :
            dir_(dir), Ia_(a), Id_(d), Is_(s) {}
        void Send(GLuint prgm) {
            GLint loc;
            vec3 v;
            loc  = glGetUniformLocation(prgm, "Ia");
            v = Ia_;
            glUniform3f(loc, v.x, v.y, v.z);
            loc  = glGetUniformLocation(prgm, "Id");
            v = Id_;
            glUniform3f(loc, v.x, v.y, v.z);
            loc  = glGetUniformLocation(prgm, "Is");
            v = Is_;
            glUniform3f(loc, v.x, v.y, v.z);
        }
        vec3 Dir() { return dir_; }
        vec3 Ia() { return Ia_; }
        vec3 Id() { return Id_; }
        vec3 Is() { return Is_; }
        void Ia(vec3 v) { Ia_ = v; }
        void Id(vec3 v) { Id_ = v; }
        void Is(vec3 v) { Is_ = v; }

    protected:
        vec3 dir_;
        vec3 Ia_;
        vec3 Id_;
        vec3 Is_;
};

#endif  //  SRC_INCLUDE_LIGHTS_H_
