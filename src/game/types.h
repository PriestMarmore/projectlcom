typedef struct {
    int x, y;
    Sprite* sprite;
} Player;

typedef struct {
    int x, y;
    int alive;
    Sprite* sprite;
} Enemy;

typedef struct {
    int x, y;
    int active;
} Bullet;