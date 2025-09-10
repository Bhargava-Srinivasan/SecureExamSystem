# database.py
import mysql.connector

DB_CONFIG = {
    "host": "localhost",
    "user": "root",
    "password": "Hnakxr12#",
    "database": "exam_system"
}

def get_user_role(username, password):
    """Returns the role of the user if credentials are valid."""
    conn = mysql.connector.connect(**DB_CONFIG)
    cursor = conn.cursor()
    cursor.execute("SELECT role FROM users WHERE username=%s AND password=%s", (username, password))
    user = cursor.fetchone()
    conn.close()
    return user[0] if user else None

def add_exam(question, answer):
    """Adds a new exam question to the database."""
    conn = mysql.connector.connect(**DB_CONFIG)
    cursor = conn.cursor()
    cursor.execute("INSERT INTO exams (question, answer) VALUES (%s, %s)", (question, answer))
    conn.commit()
    conn.close()

def get_all_exams():
    """Fetches all exams."""
    conn = mysql.connector.connect(**DB_CONFIG)
    cursor = conn.cursor()
    cursor.execute("SELECT id, question FROM exams")
    exams = cursor.fetchall()
    conn.close()
    return exams

def submit_answer(username, exam_id, answer):
    """Saves the student's submission."""
    conn = mysql.connector.connect(**DB_CONFIG)
    cursor = conn.cursor()
    cursor.execute("INSERT INTO exam_submissions (username, exam_id, answer) VALUES (%s, %s, %s)", 
                   (username, exam_id, answer))
    conn.commit()
    conn.close()

def add_student(username, password):
    """Adds a new student user."""
    conn = mysql.connector.connect(**DB_CONFIG)
    cursor = conn.cursor()
    cursor.execute("INSERT INTO users (username, password, role) VALUES (%s, %s, 'student')", (username, password))
    conn.commit()
    conn.close()

def delete_exam(exam_id):
    try:
        conn = mysql.connector.connect(**DB_CONFIG)
        cursor = conn.cursor()
        cursor.execute("DELETE FROM exams WHERE id = %s", (int(exam_id),))
        conn.commit()
        conn.close()
    except Exception as e:
        print(f"[ERROR] Failed to delete exam: {e}")

def evaluate_submission(username, exam_id):
    """Evaluates if the answer matches the correct one."""
    conn = mysql.connector.connect(**DB_CONFIG)
    cursor = conn.cursor()

    cursor.execute("SELECT answer FROM exams WHERE id = %s", (exam_id,))
    correct_result = cursor.fetchone()
    if not correct_result:
        print(f"No correct answer found for exam ID {exam_id}")
        conn.close()
        return False

    correct = correct_result[0]

    cursor.execute("SELECT answer FROM exam_submissions WHERE username = %s AND exam_id = %s",
                   (username, exam_id))
    submitted_result = cursor.fetchone()
    if not submitted_result:
        print(f"No submission found for user '{username}' and exam ID {exam_id}")
        conn.close()
        return False

    submitted = submitted_result[0]

    conn.close()

    # Debug log
    print(f"Correct: '{correct}' | Submitted: '{submitted}'")

    return submitted.strip().lower() == correct.strip().lower()

