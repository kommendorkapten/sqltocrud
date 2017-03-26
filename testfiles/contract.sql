CREATE TABLE contract(
        manager_id INTEGER NOT NULL,
        employee_id INTEGER NOT NULL,
        valid BOOLEAN,
        PRIMARY KEY(manager_id, employee_id),
        CONSTRAINT fk_manager_id FOREIGN KEY (manager_id)
                REFERENCES manager(id)
                ON UPDATE CASCADE
                ON DELETE RESTRICT,
        CONSTRAINT fk_employee_id FOREIGN KEY (employee_id)
                REFERENCES employee(id)
                ON UPDATE CASCADE
                ON DELETE RESTRICT);
