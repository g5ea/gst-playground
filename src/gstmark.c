#include "gstmark.h"

GType gst_meta_marking_api_get_type(void) {
  static volatile GType type;
  static const gchar *tags[] = {NULL};

  if (g_once_init_enter(&type)) {
    GType _type = gst_meta_api_type_register("GstMetaMarkingAPI", tags);
    g_once_init_leave(&type, _type);
  }
  return type;
}

gboolean gst_meta_marking_init(GstMeta *meta, gpointer params,
                               GstBuffer *buffer) {
  GstMetaMarking *marking_meta = (GstMetaMarking *)meta;

  marking_meta->in_timestamp.t = 0;
  marking_meta->in_timestamp.milli = 0;

  return TRUE;
}

gboolean gst_meta_marking_transform(GstBuffer *dest_buf, GstMeta *src_meta,
                                    GstBuffer *src_buf, GQuark type,
                                    gpointer data) {
  GstMeta *dest_meta = (GstMeta *)(GST_META_MARKING_ADD(dest_buf));

  GstMetaMarking *src_meta_marking = (GstMetaMarking *)src_meta;
  GstMetaMarking *dest_meta_marking = (GstMetaMarking *)dest_meta;

  dest_meta_marking->in_timestamp.t = src_meta_marking->in_timestamp.t;
  dest_meta_marking->in_timestamp.milli = src_meta_marking->in_timestamp.milli;

  return TRUE;
}

void gst_meta_marking_free(GstMeta *meta, GstBuffer *buffer) {}

const GstMetaInfo *gst_meta_marking_get_info(void) {
  static const GstMetaInfo *meta_info = NULL;

  if (g_once_init_enter(&meta_info)) {
    const GstMetaInfo *meta = gst_meta_register(
        gst_meta_marking_api_get_type(), "GstMetaMarking",
        sizeof(GstMetaMarking), (GstMetaInitFunction)gst_meta_marking_init,
        (GstMetaFreeFunction)gst_meta_marking_free,
        (GstMetaTransformFunction)gst_meta_marking_transform);
    g_once_init_leave(&meta_info, meta);
  }
  return meta_info;
}
