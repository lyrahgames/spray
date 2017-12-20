#include "glut_app.h"

namespace spray {
namespace glut_app {

state data{};
ray_tracer::kernel kernel;

void init(int argc, char** argv) {
  if (argc != 2) {
    std::cout << "usage: spray <file name>" << std::endl;
    exit(0);
  }

  kernel.s = ray_tracer::load_stl(argv[1]);
  kernel.s.build_morton_bvh();

  ray_tracer::aabb bounding_box = ray_tracer::bounds(kernel.s);

  data.opengl_rendering = false;
  data.clear_color = Eigen::Vector3f(0.2f, 0.2f, 0.2f);
  data.fps_meter = chrono::fps_meter(3.0f);
  kernel.cam.set_screen_resolution(320, 320);
  kernel.cam.set_field_of_view(0.5f * M_PI);
  kernel.rng = std::mt19937(std::random_device{}());
  kernel.reset();
  data.eye_altitude = 0.0f;
  data.eye_azimuth = 0.0f;
  const float scene_radius = ray_tracer::radius(bounding_box);
  data.eye_distance =
      scene_radius / std::sin(0.5f * kernel.cam.field_of_view());
  data.world =
      ray_tracer::blender_orthonormal_frame(ray_tracer::center(bounding_box));

  std::cout << "primitive count: " << kernel.s.primitive_data.size()
            << std::endl;
  std::cout << "bvh node count: " << kernel.s.bvh.node_data.size() << std::endl;
  std::cout << "scene min: " << bounding_box.min.transpose() << std::endl
            << "scene max: " << bounding_box.max.transpose() << std::endl
            << "scene radius: " << ray_tracer::radius(bounding_box)
            << std::endl;
  std::cout << "world origin: " << data.world.origin().transpose() << std::endl;
  std::cout << "camera distance: " << data.eye_distance << std::endl;

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  // glutInitWindowPosition(100, 100);
  glutInitWindowSize(kernel.cam.screen_width(), kernel.cam.screen_height());
  glutCreateWindow("spray: simulated particle ray tracer");

  // glutCloseFunc(close);
  glutDisplayFunc(render);
  glutReshapeFunc(resize);
  // glutIdleFunc(idle);
  glutIdleFunc(render);
  glutKeyboardFunc(process_normal_keys);
  glutSpecialFunc(process_special_keys);
  glutMouseFunc(process_mouse_buttons);
  glutPassiveMotionFunc(process_passive_mouse_move);
  glutMotionFunc(process_mouse_move);
  // glutMouseWheelFunc(process_mouse_wheel);

  // glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,
  // GLUT_ACTION_GLUTMAINLOOP_RETURNS);

  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  compute_camera_frame();
}

void exec() { glutMainLoop(); }

void close() {}

void resize(int width, int height) {
  kernel.cam.set_screen_resolution(width, height);
  // kernel.reset();
  compute_camera_frame();

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glViewport(0, 0, kernel.cam.screen_width(), kernel.cam.screen_height());
  gluPerspective(kernel.cam.opengl_field_of_view(), kernel.cam.aspect_ratio(),
                 0.1f, 1000.0f);
  glMatrixMode(GL_MODELVIEW);
}

void render() {
  data.fps_meter.measure();

  if (data.opengl_rendering) {
    render_with_opengl();
  } else {
    render_with_ray_tracer();
  }

  glutSwapBuffers();
}

void idle() {}

void process_normal_keys(unsigned char key, int x, int y) {
  switch (key) {
    case glut_key_esc:
      exit(0);
      break;
    case glut_key_b:
      data.eye_azimuth = data.eye_altitude = 0.0f;
      data.world = ray_tracer::blender_orthonormal_frame(data.world.origin());
      break;
    case glut_key_g:
      data.eye_azimuth = data.eye_altitude = 0.0f;
      data.world = ray_tracer::opengl_orthonormal_frame(data.world.origin());
      break;
    case glut_key_w:
      data.opengl_rendering = !data.opengl_rendering;
      break;
  }

  compute_camera_frame();
}

void process_special_keys(int key, int x, int y) {
  switch (key) {
    case GLUT_KEY_LEFT:
      data.eye_azimuth += eye_azimuth_key_inc;
      break;
    case GLUT_KEY_RIGHT:
      data.eye_azimuth -= eye_azimuth_key_inc;
      break;
    case GLUT_KEY_UP:
      data.eye_altitude += eye_altitude_key_inc;
      break;
    case GLUT_KEY_DOWN:
      data.eye_altitude -= eye_altitude_key_inc;
      break;
  }
  compute_camera_frame();
  glutPostRedisplay();
}

void process_mouse_buttons(int button, int button_state, int x, int y) {
  data.key_modifiers = glutGetModifiers();
}

void process_mouse_move(int x, int y) {
  const float x_difference = static_cast<float>(x - data.old_mouse_x);
  const float y_difference = static_cast<float>(y - data.old_mouse_y);

  switch (data.key_modifiers) {
    // zooming
    case GLUT_ACTIVE_CTRL:
      data.eye_distance +=
          eye_distance_inc_scale * y_difference * data.eye_distance;
      if (data.eye_distance < eye_distance_min)
        data.eye_distance = eye_distance_min;
      break;

    // moving
    case GLUT_ACTIVE_SHIFT:
      data.world.set_origin(data.world.origin() -
                            kernel.cam.frame().right() * x_difference *
                                kernel.cam.pixel_size() * data.eye_distance +
                            kernel.cam.frame().up() * y_difference *
                                kernel.cam.pixel_size() * data.eye_distance);
      break;

    // rotating
    default:
      data.eye_azimuth -= x_difference * eye_azimuth_inc_scale;
      data.eye_altitude += y_difference * eye_altitude_inc_scale;
      if (data.eye_altitude > eye_altitude_max_abs)
        data.eye_altitude = eye_altitude_max_abs;
      if (data.eye_altitude < -eye_altitude_max_abs)
        data.eye_altitude = -eye_altitude_max_abs;
  }

  compute_camera_frame();

  data.old_mouse_x = x;
  data.old_mouse_y = y;
}

void process_passive_mouse_move(int x, int y) {
  data.old_mouse_x = x;
  data.old_mouse_y = y;
}

void process_mouse_wheel(int wheel, int direction, int x, int y) {}

void compute_camera_frame() {
  kernel.cam.look_at(
      ray_tracer::horizontal_coordinates(data.world, data.eye_distance,
                                         data.eye_altitude, data.eye_azimuth),
      data.world.origin(), data.world.up());
  kernel.cam.set_horizontal_field_of_view(M_PI_2);
  kernel.reset();

  glLoadIdentity();
  gluLookAt(kernel.cam.position()(0), kernel.cam.position()(1),
            kernel.cam.position()(2), data.world.origin()(0),
            data.world.origin()(1), data.world.origin()(2), data.world.up()(0),
            data.world.up()(1), data.world.up()(2));
}

void render_with_ray_tracer() {
  kernel.clear_color = data.clear_color;
  // kernel.render();
  kernel.render_bvh();
  glDrawPixels(kernel.cam.screen_width(), kernel.cam.screen_height(), GL_RGBA,
               GL_FLOAT, kernel.accum_buffer.data());
}

void render_with_opengl() {
  glClearColor(data.clear_color(0), data.clear_color(1), data.clear_color(2),
               1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  for (int i = 0; i < static_cast<int>(kernel.s.primitive_data.size()); ++i) {
    const ray_tracer::scene::primitive& p = kernel.s.primitive_data[i];

    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3fv(reinterpret_cast<GLfloat*>(
        kernel.s.vertex_data[p.vertex_id[0]].position.data()));
    glVertex3fv(reinterpret_cast<GLfloat*>(
        kernel.s.vertex_data[p.vertex_id[1]].position.data()));
    glVertex3fv(reinterpret_cast<GLfloat*>(
        kernel.s.vertex_data[p.vertex_id[2]].position.data()));
    glEnd();
  }
}

}  // namespace glut_app
}  // namespace spray