#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../testfiles/contract.h"
#include "../testfiles/employee.h"
#include "../testfiles/manager.h"

#ifndef DBFILE
# define DBFILE "test.db"
#endif

sqlite3* db;

void test_create_destroy(void);
void test_select_empty(void);
void test_insert_select(void);
void test_update(void);
void test_delete(void);

int main(void)
{
        if (sqlite3_open(DBFILE, &db))
        {
                printf("Can not open %s\n", DBFILE);
                exit(-1);
        }

        printf("Test create/destroy...");
        fflush(stdout);
        test_create_destroy();
        printf("Ok\n");

        printf("Test select empty set...");
        fflush(stdout);
        test_select_empty();
        printf("Ok\n");

        printf("Test insert and select...");
        fflush(stdout);
        test_insert_select();
        printf("Ok\n");

        printf("Test update...");
        fflush(stdout);
        test_update();
        printf("Ok\n");

        printf("Test delete...");
        fflush(stdout);
        test_delete();
        printf("Ok\n");

        sqlite3_close(db);

        return 0;
}

void test_create_destroy(void)
{
        struct contract* cnt = contract_alloc();
        struct employee* emp = employee_alloc();
        struct manager* mgr = manager_alloc();

        if (cnt == NULL)
        {
                printf("Contract was NULL\n");
                exit(-1);
        }
        if (emp == NULL)
        {
                printf("Employee was NULL\n");
                exit(-1);
        }
        if (mgr == NULL)
        {
                printf("Manager was NULL\n");
                exit(-1);
        }
        if (emp->fname)
        {
                printf("Employee fname is not NULL\n");
                exit(-1);
        }
        if (emp->lname)
        {
                printf("Employee lname is not NULL\n");
                exit(-1);
        }
        if (mgr->fname)
        {
                printf("Manager fname is not NULL\n");
                exit(-1);
        }
        if (mgr->lname)
        {
                printf("Manager lname is not NULL\n");
                exit(-1);
        }

        contract_free(cnt);
        employee_free(emp);
        manager_free(mgr);
}

void test_select_empty(void)
{
        struct contract* cnt = contract_alloc();
        struct employee* emp = employee_alloc();
        struct manager* mgr = manager_alloc();
        int ret;

        cnt->manager_id = 12;
        cnt->employee_id = 13;
        emp->id = 14;
        mgr->id = 15;

        ret = contract_read(db, cnt);
        if (ret < 1)
        {
                printf("Wrong code reading non-existing contract: %d\n", ret);
                exit(-1);
        }

        ret = employee_read(db, emp);
        if (ret < 1)
        {
                printf("Wrong code reading non-existing employee: %d\n", ret);
                exit(-1);
        }

        ret = manager_read(db, mgr);
        if (ret < 1)
        {
                printf("Wrong code reading non-existing manager: %d\n", ret);
                exit(-1);
        }

        contract_free(cnt);
        employee_free(emp);
        manager_free(mgr);
}

void test_insert_select(void)
{
        struct contract* cnt = contract_alloc();
        struct employee* emp = employee_alloc();
        struct manager* mgr = manager_alloc();
        int ret;

        cnt->manager_id = 100;
        cnt->employee_id = 200;
        cnt->valid = 1;

        emp->id = 200;
        emp->fname = "First E";
        emp->lname = "Last E";
        mgr->id = 100;

        mgr->fname = "First M";
        mgr->lname = "Last M";
        mgr->shoe_size = 43.4f;

        ret = employee_create(db, emp);
        if (ret)
        {
                printf("Failed to insert employee %d\n", ret);
                exit(-1);
        }

        ret = manager_create(db, mgr);
        if (ret)
        {
                printf("Failed to insert manager %d\n", ret);
                exit(-1);
        }

        ret = contract_create(db, cnt);
        if (ret)
        {
                printf("Failed to insert contract %d\n", ret);
                exit(-1);
        }

        emp->fname = NULL;
        emp->lname = NULL;
        mgr->fname = NULL;
        mgr->lname = NULL;
        mgr->shoe_size = 0;
        cnt->valid = 0;

        ret = contract_read(db, cnt);
        if (ret)
        {
                printf("Wrong code reading existing contract: %d\n", ret);
                exit(-1);
        }

        ret = employee_read(db, emp);
        if (ret)
        {
                printf("Wrong code reading existing employee: %d\n", ret);
                exit(-1);
        }

        ret = manager_read(db, mgr);
        if (ret)
        {
                printf("Wrong code reading existing manager: %d\n", ret);
                exit(-1);
        }

        if (strcmp(emp->fname, "First E"))
        {
                printf("Wrong first name for employee '%s'\n", emp->fname);
                exit(-1);
        }
        if (strcmp(emp->lname, "Last E"))
        {
                printf("Wrong last name for employee '%s'\n", emp->lname);
                exit(-1);
        }

        if (strcmp(mgr->fname, "First M"))
        {
                printf("Wrong first name for manager '%s'\n", mgr->fname);
                exit(-1);
        }
        if (strcmp(mgr->lname, "Last M"))
        {
                printf("Wrong first name for manager '%s'\n", mgr->lname);
                exit(-1);
        }
        if (mgr->shoe_size - 43.4f > 0.001)
        {
                printf("Wrong shoe size for manager: %f\n", mgr->shoe_size);
                exit(-1);
        }
        if (cnt->valid != 1)
        {
                printf("Wrong valid for contract: %d\n", cnt->valid);
        }

        contract_free(cnt);
        employee_free(emp);
        manager_free(mgr);
}

void test_update(void)
{
        struct contract* cnt = contract_alloc();
        struct employee* emp = employee_alloc();
        struct manager* mgr = manager_alloc();
        int ret;

        cnt->manager_id = 100;
        cnt->employee_id = 200;
        cnt->valid = 4;

        emp->id = 200;
        emp->fname = "First E Changed 1";
        emp->lname = "Last E Changed 2";
        mgr->id = 100;

        mgr->fname = "First M 3";
        mgr->lname = "Last M 4";
        mgr->shoe_size = 100.0f;

        ret = employee_update(db, emp);
        if (ret)
        {
                printf("Failed to update employee %d\n", ret);
                exit(-1);
        }

        ret = manager_update(db, mgr);
        if (ret)
        {
                printf("Failed to update manager %d\n", ret);
                exit(-1);
        }

        ret = contract_update(db, cnt);
        if (ret)
        {
                printf("Failed to update contract %d\n", ret);
                exit(-1);
        }

        emp->fname = NULL;
        emp->lname = NULL;
        mgr->fname = NULL;
        mgr->lname = NULL;
        cnt->valid = 0;

        ret = contract_read(db, cnt);
        if (ret)
        {
                printf("Wrong code reading existing contract: %d\n", ret);
                exit(-1);
        }

        ret = employee_read(db, emp);
        if (ret)
        {
                printf("Wrong code reading existing employee: %d\n", ret);
                exit(-1);
        }

        ret = manager_read(db, mgr);
        if (ret)
        {
                printf("Wrong code reading existing manager: %d\n", ret);
                exit(-1);
        }

        if (strcmp(emp->fname, "First E Changed 1"))
        {
                printf("Wrong first name for employee '%s'\n", emp->fname);
                exit(-1);
        }
        if (strcmp(emp->lname, "Last E Changed 2"))
        {
                printf("Wrong last name for employee '%s'\n", emp->lname);
                exit(-1);
        }

        if (strcmp(mgr->fname, "First M 3"))
        {
                printf("Wrong first name for manager '%s'\n", mgr->fname);
                exit(-1);
        }
        if (strcmp(mgr->lname, "Last M 4"))
        {
                printf("Wrong first name for manager '%s'\n", mgr->lname);
                exit(-1);
        }
        if (mgr->shoe_size - 100.0f > 0.001)
        {
                printf("Wrong shoe size for manager: %f\n", mgr->shoe_size);
                exit(-1);
        }
        if (cnt->valid != 4)
        {
                printf("Wrong valid for contract: %d\n", cnt->valid);
        }

        contract_free(cnt);
        employee_free(emp);
        manager_free(mgr);
}

void test_delete(void)
{
        struct contract* cnt = contract_alloc();
        struct employee* emp = employee_alloc();
        struct manager* mgr = manager_alloc();
        int ret;

        emp->id = 200;
        mgr->id = 100;
        cnt->manager_id = 100;
        cnt->employee_id = 200;
        cnt->valid = 4;

        ret = contract_delete(db, cnt);
        if (ret)
        {
                printf("Failed to delete contract: %d\n", ret);
        }
        ret = employee_delete(db, emp);
        if (ret)
        {
                printf("Failed to delete employee: %d\n", ret);
        }
        ret = manager_delete(db, mgr);
        if (ret)
        {
                printf("Failed to delete manager: %d\n", ret);
        }

        ret = contract_read(db, cnt);
        if (ret < 1)
        {
                printf("Can still read contract: %d\n", ret);
                exit(-1);
        }

        ret = employee_read(db, emp);
        if (ret < 1)
        {
                printf("Can still read employee: %d\n", ret);
                exit(-1);
        }

        ret = manager_read(db, mgr);
        if (ret < 1)
        {
                printf("Can still read manager: %d\n", ret);
                exit(-1);
        }

        contract_free(cnt);
        employee_free(emp);
        manager_free(mgr);
}
