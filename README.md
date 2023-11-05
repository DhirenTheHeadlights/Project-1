# Project-1
**The Savant LLC Game Development Project**

## Prerequisites

- **SFML**: Ensure you have the latest version of SFML installed on your computer. If not, download it from the official site [here](https://www.sfml-dev.org/download.php).

## Setting Up SFML_PATH Environment Variable

To streamline the setup process, we will be setting an environment variable named `SFML_PATH`. This variable will point to the root directory of your SFML installation.

1. Right-click on `This PC` or `Computer` on the desktop or in File Explorer, then choose `Properties`.
2. Click on `Advanced system settings` on the left panel.
3. Click on the `Environment Variables` button under the Advanced tab.
4. Under the `System Variables` section, click the `New` button.
5. For the variable name, enter `SFML_PATH`.
6. For the variable value, enter the path to the root of your SFML directory (e.g., `C:\SFML-2.5.1`).
7. Click `OK` on all dialog boxes to save and exit.

## Visual Studio Configuration

- **Include Directories**:
  1. Go to `Project -> Properties -> C++ -> General -> Additional Include Directories`.
  2. Add `$(SFML_PATH)\include`.
     
- **Lib Directories**:
  1. Go to `Project -> Properties -> Linker -> General -> Additional Library Directories`.
  2. Add `$(SFML_PATH)\lib`.
     
- **Linking Libraries**:
  1. Go to `Project -> Properties -> Linker -> Input -> Additional Dependencies`.
  2. Make sure the names of the required SFML `.lib` files, like `sfml-graphics.lib`, `sfml-window.lib`, etc are in there. Note: If you're statically linking, ensure you link against the `-s` suffixed versions, e.g., `sfml-graphics-s.lib`.
  
Make sure you run the program in "Release" configuration. It will not run in "Debug" configuration.

## Troubleshooting

1. **Environment Variable**: Ensure the `SFML_PATH` environment variable is correctly set.

2. **Directory Paths**: Verify the paths in Visual Studio settings are correct.

3. **SFML Version**: Ensure you're using the correct version.

4. **Missing DLLs**: If using dynamic linking, ensure the necessary `.dll` files are copied to your output directory.

5. **Platform Target Mismatch**: Ensure your project's target platform matches the SFML binaries.

6. **Statically Linking Libraries**: 
   - If you're linking SFML libraries statically, ensure you've defined `SFML_STATIC` in `Project -> Properties -> C/C++ -> Preprocessor -> Preprocessor Definitions`.
   - Ensure you're linking against the static runtime by going to `Project -> Properties -> C/C++ -> Code Generation` and set `Runtime Library` to `/MT` (or `/MTd` for debug).

7. **Rebuild**: Clean and rebuild the project.

8. **Library Linking**: Check that you've linked all the required libraries, especially if getting linker errors. Make sure you're not mixing debug and release libraries.

9. **Official Resources**: Check the [official SFML forums](https://en.sfml-dev.org/forums/).

