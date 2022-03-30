# ctf-tool
A C++ tool that automates CTF flag finding.

## Configuration
The tool reads a file called `config.ini` in the directory one above the binary.

Example contents:
```
key: password
format: ctf{.*}
input: PRsmCTg7Px49Gz4JOTs/HT0bKkE5Kz9WPRsmCToVPx09GyIJOQU/Vj0bGAk5Oz9RPRs+CToFP1U9GzoJOQUrHj0bJkE6Oz9WPgs2CTkrP1U9GxAJODs/VD4LIgk5FSscPRsmCTo7Px49GzYJOSsrVT0bKkE5Oz8ePRsyCToVK1Q=
num_workers: 4
```

(This is a concatenation of 6 operations currently supported by the tool.)

- Inspired by [katana](https://github.com/JohnHammond/katana)
- Inspired by [cyberchef](https://github.com/gchq/CyberChef)