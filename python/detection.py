class Detection:
    def __init__(self, class_name: str, obj_id: int, pos: tuple):
        self.class_name = class_name
        self.obj_id = obj_id
        self.pos = pos

    def __str__(self):
        return f"{self.class_name} {self.obj_id} : {self.pos}"

    def __repr__(self):
        return str(self)
