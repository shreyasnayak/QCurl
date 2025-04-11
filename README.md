# QCurl

QCurl is a lightweight QT C++ implementation of the cURL functionality using Qt libraries. It is designed to help debug HTTP API issues in QT applications, particularly those arising from firewall and environmental constraints that can obscure whether requests are reaching the server. While QCurl is not intended to replace cURL, it serves as a valuable debugging tool to emulate HTTP requests using native Qt functionality.

### Prerequisites

- Qt 5.9 or later
- A C++11 compatible compiler

### Building QCurl

1. **Clone the repository:**
   ```bash
   git clone https://github.com/shreyasnayak/QCurl.git
   ```

2. **Navigate to the project directory:**
   ```bash
   cd QCurl
   ```

3. **Build the project using QMake and your preferred build tool:**
   ```bash
   qmake QCurl.pro
   make
   ```

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for more details.

## Contributing

Contributions are welcome! Please fork the repository and submit a pull request with your improvements.

## Contact

For questions or suggestions, please open an issue on the [GitHub repository](https://github.com/shreyasnayak/QCurl.git).

