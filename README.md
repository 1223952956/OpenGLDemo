## Dependencies

Third-party libraries are not included in this repository.

Download the dependencies package from the following link:

https://drive.google.com/file/d/1o8tYNo5nUCJQnEL3vmUxfm08SfcXYmsR/view?usp=sharing

## Build Instructions

After downloading the dependencies, follow these steps:

1. Extract `ThirdParty.zip`.
2. Copy the extracted `bin`, `include`, and `libs` folders directly into the root directory of this repository.
3. Open `OpenGLDemo.sln` and build the project once.

   * The first build is expected to fail because the required runtime files are not yet present in the output directory.
   * This step generates the `x64/Debug/` output directory.
4. Copy `assimp-vc142-mtd.dll` from the `bin/` folder to `x64/Debug/`.
5. Copy the entire `content/` folder to `x64/Debug/`.
6. Build the project again. The application should now compile and run successfully.



