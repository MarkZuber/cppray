#include "cppray.h"

int Shape::_nextId = 1;

Shape::Shape(const POS_VECTOR &position, const shared_ptr<Material> &material)
{
    _id = ++_nextId;
    _position = position;
    _material = material;
}

int Shape::Id() const
{
    return _id;
}

shared_ptr<Material> Shape::GetMaterial() { return _material; }
POS_VECTOR Shape::Position() const { return _position; }

SphereShape::SphereShape(const POS_VECTOR &position, double radius, const shared_ptr<Material> &material) : Shape(position, material), _radius(radius) {}

shared_ptr<IntersectionInfo> SphereShape::Intersect(const Ray &ray)
{
    shared_ptr<IntersectionInfo> info = make_shared<IntersectionInfo>();
    info->SetElement(std::enable_shared_from_this<Shape>::shared_from_this());

    POS_VECTOR dst = ray.Position() - Position();
    double b = dot(dst, ray.Direction());
    double c = dot(dst, dst) - (_radius * _radius);
    double d = b * b - c;

    if (d > 0.0) // yes, that's it, we found the intersection!
    {
        // Console.WriteLine("sphere intersect");
        info->SetIsHit(true);
        info->SetDistance(-b - sqrt(d));
        info->SetPosition(ray.Position() + ray.Direction() * info->Distance());
        POS_VECTOR norm = info->Position() - Position();
        normalize(norm);
        info->SetNormal(norm);

        if (GetMaterial()->HasTexture())
        {
            // north pole / up
            POS_VECTOR vn = {0.0, 1.0, 0.0};
            normalize(vn);

            // equator / sphere orientation
            POS_VECTOR ve = {0.0, 0.0, 1.0};
            normalize(ve);

            //points from center of sphere to intersection
            POS_VECTOR vp = info->Position() - Position();
            normalize(vp);

            double phi = acos(-dot(vp, vn));
            double v = (phi * 2.0 / boost::math::constants::pi<double>()) - 1.0;

            double sinphi = dot(ve, vp) / sin(phi);
            sinphi = sinphi < -1.0 ? -1.0 : sinphi > 1.0 ? 1.0 : sinphi; // todo: clamp?
            double theta = acos(sinphi) * 2.0 / boost::math::constants::pi<double>();

            double u;

            if (dot(cross(vn, ve), vp) > 0.0)
            {
                u = theta;
            }
            else
            {
                u = 1.0 - theta;
            }

            // alternative but worse implementation
            //double u = Math.Atan2(vp.x, vp.z);
            //double v = Math.Acos(vp.y);
            info->SetColor(GetMaterial()->GetColor(u, v));
            // Console.WriteLine($"uv -> {u},{v} => {info.Color}");
        }
        else
        {
            // skip uv calculation, just get the color
            info->SetColor(GetMaterial()->GetColor(0.0, 0.0));
        }
    }
    else
    {
        info->SetIsHit(false);
    }
    return info;
}
