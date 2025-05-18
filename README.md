# Diplomski-Projekat

This repository contains the source code and documentation for a diploma thesis project. It features a **C# graphical user interface** and a **C++ backend**, where the C# application programmatically launches the C++ executable.

---

## 🗂️ Project Structure

```
Diplomski-Projekat/
├── Projekat/          # C++ backend (core logic)
├── Projekat_UI/       # C# frontend (user interface)
├── Documentation/     # Design docs, thesis materials
```

---

## 🚀 Getting Started

### ✅ Prerequisites

- [.NET SDK](https://dotnet.microsoft.com/download) (for the C# UI)
- A C++ compiler (e.g., MSVC, g++, clang++)

---

### ⚙️ Building the Project

1. **Clone the repository:**

   ```bash
   git clone https://github.com/ininic/Diplomski-Projekat.git
   cd Diplomski-Projekat
   ```

2. **Build the C++ backend:**

   ```bash
   cd Projekat
   C++ build
   ```

   
3. **Build and run the C# UI:**

   ```bash
   cd ../Projekat_UI
   dotnet build
   dotnet run
   ```

---

## 🔄 How Communication Works

The C# UI and the C++ backend exchange information through text files:

- The **C# app** writes data to a files.
- It then launches the C++ app using `System.Diagnostics.Process`.
- The **C++ app** reads the input file, processes the data, and writes the result to an output files.
- After execution, the C# app reads the output files and displays and processes the result.

---

## 📄 Documentation

All project documentation, diagrams, and written materials are available in the `Documentation/` folder.

---

