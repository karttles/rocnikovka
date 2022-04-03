from flask import Flask, render_template, request
from arduion_logic import send_msg


app = Flask(__name__)
selected_box = "a1"
target_box = "a1"

@app.route("/", methods=["POST", "GET"])

def index():
    if request.method == "POST":
        selected_box = request.form["selected"]
        move_box = request.form["submit_move"]
        target_box = request.form["target"]
        if move_box == "move":
                send_msg(f"move {selected_box}")
                send_msg("nalozit")
                send_msg(f"move {target_box}")
                send_msg("vylozit")
                return render_template('index.html')
        else:
            pass
    else:
        return render_template('index.html')


if __name__ == '__main__':
    app.run(host='0.0.0.0', debug=True)
