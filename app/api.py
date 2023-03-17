from flask_restful import Resource, Api
from flask import Flask, request
from flask_cors import CORS

# This was hardcoded in the `discoveryserver/WebSocketMainWS.cpp`::CheckURL
# You could set this to something safer like (I think) JWT, short code?
SECERT_KEY = "z/EahGU31q1G5L14763UItXD6dI2X57RlUS7CI2n43g="


class checkurl(Resource):
    def post(self):
        data = request.form
        print(data)
        secertkey = data['secretkey']
        if secertkey:
            if secertkey == SECERT_KEY:
                return { "valid": True, "msg": "Welcome to LANChat"}
            else:
                return { "valid": False, "msg": "Secert key not found"}
        else:
            return { "valid": False, "msg": "Empty parameter"}

app = Flask(__name__)
CORS(app)
api = Api(app)
api.add_resource(checkurl, '/checkurl')

if __name__ == "__main__":
    app.run(host='0.0.0.0', port=8081)