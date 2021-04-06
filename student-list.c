#include <gtk/gtk.h>
#include "db/student-db.h"

static GtkWidget *window = NULL;
static guint timeout = 0;
static GtkTreeModel *model = NULL;

typedef struct
{
  const gint mssv;
  const gchar *name;
  const gchar *birth;
  const gchar *address;
}
Row;

enum
{
  COLUMN_MSSV,
  COLUMN_NAME,
  COLUMN_BIRTH,
  COLUMN_ADDRESS,
  NUM_COLUMNS
};

static student *data = NULL;
static int studentNum = 0;

// static Row data[] = 
// {
// 	{20194547, "mr ha", "19/04/01", "hahahahi"},
// 	{54542223, "mr ha2", "19/04/11", "hahahadfhi"},
// 	{20394547, "mr ha3", "19/04/02", "hahahaasdhi"},
// 	{20196547, "mr ha4", "19/04/44", "hahahahcvi"},
// 	{20194547, "mr ha5", "19/94/01", "hahahacvchi"},
// 	{20194577, "mr ha6", "19/04/01", "hahahaaahi"},
// 	{20194547, "mr ha7", "19/04/01", "hahxxxxxxahahi"},
// 	{20794547, "mr ha8", "44/04/01", "habbbbbbbbbbbbhahahi"},
// 	{29194547, "mr ha9", "33/04/01", "hahahggggggahi"},
// 	{30194547, "mr ha0", "19/94/01", "hahdfffffahahi"},
// 	{90194547, "mr ha1", "19/24/01", "hahahhhhhhhhhhhhhhhhhhhhahi"},
// };


static GtkTreeModel *
create_model (void)
{
  gint i = 0;
  GtkListStore *store;
  GtkTreeIter iter;

  /* create list store */
  store = gtk_list_store_new (NUM_COLUMNS,
                              G_TYPE_UINT,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING);

  /*-----------get Data form sqlite------------*/
  data = get_student_list();
  studentNum = get_student_num();

  /* add data to the list store */
  for (i = 0; i < studentNum; i++)
    {
      gtk_list_store_append (store, &iter);
      gtk_list_store_set (store, &iter,
                          COLUMN_MSSV, (gint)((data+i)->mssv),
                          COLUMN_NAME, (gchar *)((data+i)->name),
                          COLUMN_BIRTH, (gchar *)((data+i)->birth),
                          COLUMN_ADDRESS, (gchar *)((data+i)->address),
                          -1);
    }

  return GTK_TREE_MODEL (store);
}


static void
add_columns (GtkTreeView *treeview)
{
  GtkCellRenderer *renderer;
  GtkTreeViewColumn *column;
  GtkTreeModel *model = gtk_tree_view_get_model (treeview);

  /* column for bug numbers */
  renderer = gtk_cell_renderer_text_new ();
  column = gtk_tree_view_column_new_with_attributes ("MSSV",
                                                     renderer,
                                                     "text",
                                                     COLUMN_MSSV,
                                                     NULL);
  gtk_tree_view_column_set_sort_column_id (column, COLUMN_MSSV);
  gtk_tree_view_append_column (treeview, column);

  /* column for severities */
  renderer = gtk_cell_renderer_text_new ();
  column = gtk_tree_view_column_new_with_attributes ("Name",
                                                     renderer,
                                                     "text",
                                                     COLUMN_NAME,
                                                     NULL);
  gtk_tree_view_column_set_sort_column_id (column, COLUMN_NAME);
  gtk_tree_view_append_column (treeview, column);


  /* column for severities */
  renderer = gtk_cell_renderer_text_new ();
  column = gtk_tree_view_column_new_with_attributes ("Birth",
                                                     renderer,
                                                     "text",
                                                     COLUMN_BIRTH,
                                                     NULL);
  gtk_tree_view_column_set_sort_column_id (column, COLUMN_BIRTH);
  gtk_tree_view_append_column (treeview, column);

  /* column for description */
  renderer = gtk_cell_renderer_text_new ();
  column = gtk_tree_view_column_new_with_attributes ("Address",
                                                     renderer,
                                                     "text",
                                                     COLUMN_ADDRESS,
                                                     NULL);
  gtk_tree_view_column_set_sort_column_id (column, COLUMN_ADDRESS);
  gtk_tree_view_append_column (treeview, column);
}


GtkWidget *
do_list_store (GtkWidget *do_widget)
{
  if (!window)
    {
      GtkWidget *vbox;
      GtkWidget *label;
      GtkWidget *sw;
      GtkWidget *treeview;

      /* create window, etc */
      window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
      gtk_window_set_screen (GTK_WINDOW (window),
                             gtk_widget_get_screen (do_widget));
      gtk_window_set_title (GTK_WINDOW (window), "List Store");

      g_signal_connect (window, "destroy",
                        G_CALLBACK (gtk_widget_destroyed), &window);
      gtk_container_set_border_width (GTK_CONTAINER (window), 8);

      vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 8);
      gtk_container_add (GTK_CONTAINER (window), vbox);

      label = gtk_label_new ("This is the bug list (note: not based on real data, it would be nice to have a nice ODBC interface to bugzilla or so, though).");
      gtk_box_pack_start (GTK_BOX (vbox), label, FALSE, FALSE, 0);

      sw = gtk_scrolled_window_new (NULL, NULL);
      gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (sw),
                                           GTK_SHADOW_ETCHED_IN);
      gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (sw),
                                      GTK_POLICY_NEVER,
                                      GTK_POLICY_AUTOMATIC);
      gtk_box_pack_start (GTK_BOX (vbox), sw, TRUE, TRUE, 0);

      /* create tree model */
      model = create_model ();

      /* create tree view */
      treeview = gtk_tree_view_new_with_model (model);
      gtk_tree_view_set_search_column (GTK_TREE_VIEW (treeview),
                                       COLUMN_NAME);

      g_object_unref (model);

      gtk_container_add (GTK_CONTAINER (sw), treeview);

      /* add columns to the tree view */
      add_columns (GTK_TREE_VIEW (treeview));

      /* finish & show */
      gtk_window_set_default_size (GTK_WINDOW (window), 280, 250);
      // g_signal_connect (window, "delete-event",
      //                   G_CALLBACK (window_closed), NULL);
    }
      gtk_widget_show_all (window);


  // if (!gtk_widget_get_visible (window))
  //   {
  //     gtk_widget_show_all (window);
  //     if (timeout == 0) {
  //       /* FIXME this should use the animation-duration instead */
  //       timeout = g_timeout_add (80, spinner_timeout, NULL);
  //     }
  //   }
  // else
  //   {
  //     gtk_widget_destroy (window);
  //     window = NULL;
  //     if (timeout != 0)
  //       {
  //         g_source_remove (timeout);
  //         timeout = 0;
  //       }
  //   }

  return window;
}

int main (int argc, char *argv[]) {
  gtk_init(&argc, &argv);
  do_list_store(window);
  gtk_widget_show(window);
  gtk_main();

  return EXIT_SUCCESS;
}
