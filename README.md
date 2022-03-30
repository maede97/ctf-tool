# ctf-tool
A C++ tool that automates CTF flag finding.

## Usage
1. Download the source code from [GitHub](https://github.com/maede97/ctf-tool) directly or with `git clone https://github.com/maede97/ctf-tool.git`.
2. Compile the source code with `cmake ..` and `make`.
3. Run the executable with `./CTFTool`.
4. Edit the generated configuration file `ctf-tool.ini` in the directory above (see section below).

## Configuration
The tool reads a file called `ctf-tool.ini` in the directory one above the binary.

Example contents:
```
key: password
format: ctf\{.*\}
input: PRsmCTg7Px49Gz4JOTs/HT0bKkE5Kz9WPRsmCToVPx09GyIJOQU/Vj0bGAk5Oz9RPRs+CToFP1U9GzoJOQUrHj0bJkE6Oz9WPgs2CTkrP1U9GxAJODs/VD4LIgk5FSscPRsmCTo7Px49GzYJOSsrVT0bKkE5Oz8ePRsyCToVK1Q=
num_workers: 4
```

(This is a concatenation of 6 operations currently supported by the tool.)

## Output
The tool constantly outputs the current state and information about the search in the terminal (example snippet here, after the solution has been found):
```
   ******  ********** ********       **********                    **
  **////**/////**/// /**/////       /////**///                    /**
 **    //     /**    /**                /**      ******   ******  /**
/**           /**    /*******  *****    /**     **////** **////** /**
/**           /**    /**////  /////     /**    /**   /**/**   /** /**
//**    **    /**    /**                /**    /**   /**/**   /** /**
 //******     /**    /**                /**    //****** //******  ***
  //////      //     //                 //      //////   //////  ///

By maede97

---------------------------------------------------------------------

[!] Key: "password"
[!] Input: "PRsmCTg7Px49Gz4JOTs/HT0bKkE5Kz9WPRsmCToVPx09GyIJOQU/Vj0bGAk5Oz9RPRs+CToFP1U9GzoJOQUrHj0bJkE6Oz9WPgs2CTkrP1U9GxAJODs/VD4LIgk5FSscPRsmCTo7Px49GzYJOSsrVT0bKkE5Oz8ePRsyCToVK1Q="
[+] Found valid flag: "ctf{abcdef123456}"
[+] Backtrace:
 --> Base64Decode --> XOR --> Base64Decode --> FromHex --> FromHex --> Base64Decode
[!] Stopped worker pool
[!] Had a total of 31969 jobs

---------------------------------------------------------------------

[1] Worker completed job: "ToHex"
[2] Worker completed job: "XOR"
[3] Worker got job: "Base64Decode"
[0] Worker got job: "PassThrough"
[1] Worker got job: "FromHex"
[2] Worker got job: "ToHex"
[3] Worker completed job: "Base64Decode"

---------------------------------------------------------------------

[+] Add job: "ToHex"
[+] Add job: "XOR"
[+] Add job: "Base64Encode"
[+] Add job: "Base64Decode"
[+] Add job: "PassThrough"
[+] Add job: "FromHex"
[+] Add job: "ToHex"

---------------------------------------------------------------------
```
## Inspiration
- Inspired by [katana](https://github.com/JohnHammond/katana)
- Inspired by [cyberchef](https://github.com/gchq/CyberChef)
