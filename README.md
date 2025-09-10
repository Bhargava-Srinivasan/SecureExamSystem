# SecureExamSystem

A GUI-based **Online Examination System with Secure Access**, built using Python and MySQL.  
This project ensures secure role-based access for students and administrators, real-time evaluation of answers, and strong encryption for data transmission.

## 🚀 Features
- **Role-Based Access Control**  
  - Admins: Add/Delete exam questions, register students.  
  - Students: Take exams and receive instant evaluation.  

- **Security Features**  
  - Password masking during login.  
  - RSA encryption for sensitive data (credentials).  
  - AES encryption for session data and responses.  
  - Public/Private Key Infrastructure (PKI) for secure key management.  
  - Encrypted transmission combining RSA (asymmetric) and AES (symmetric) methods.  

- **User-Friendly Interface**  
  - Built with Tkinter and enhanced using ttkbootstrap.  
  - Intuitive dashboards for both admins and students.  

## 🛠️ Tech Stack
- **Frontend/GUI:** Python (Tkinter, ttkbootstrap)  
- **Backend:** MySQL  
- **Encryption:** RSA, AES  
- **Libraries:** `mysql-connector-python`, `tkinter`, `ttkbootstrap`

## 📂 Project Structure
- `main.py` → Entry point of the application  
- `gui.py` → Login interface  
- `admin.py` → Admin dashboard  
- `student.py` → Student dashboard  
- `database.py` → Handles MySQL operations and authentication  

## ⚙️ Requirements
- Python 3.8+  
- MySQL Server 5.7+  
- Dependencies:

## 🔒 Security Architecture

RSA public/private keys (public.pem, private.pem) for credential encryption/decryption.

AES used for efficient session data security.
  ```bash
  pip install mysql-connector-python ttkbootstrap
