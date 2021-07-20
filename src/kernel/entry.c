
// Kernel Entrypoint
void _start() {
    int* video_memory = (int*)0xb8000;
    *video_memory = 0x50505050;

    return;
}
